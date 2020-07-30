# https://www.tomdalling.com/toms-data-onion/
import base64
import binascii
import hashlib
import ipaddress
import itertools
import sys
from aes_keywrap import aes_unwrap_key
from Crypto.Cipher import AES

def get_payload(lines):
    try:
        index = lines.index('==[ Payload ]===============================================\n')
    except ValueError:
        return None
    return ''.join(lines[index + 1:]).strip()

def layer0(payload):
    return base64.a85decode(payload, adobe=True).decode()

def rotate_right_by_one(byte):
    return ((byte & 0x1) << 7) | (byte >> 1)

def layer1(payload):
    decoded = base64.a85decode(payload, adobe=True)
    return bytes(rotate_right_by_one(byte ^ 0b01010101) for byte in decoded).decode()

def popcnt(byte):
    return bin(byte).count('1')

def parity_check(byte):
    return popcnt(byte >> 1) % 2 == (byte & 0x1)

def layer2(payload):
    decoded = filter(parity_check, base64.a85decode(payload, adobe=True))
    decoded = [byte & ~0x1 for byte in decoded]
    chunks = (decoded[i:i+8] for i in range(0, len(decoded), 8))
    result = bytearray()
    for chunk in chunks:
        for i in range(7):
            result.append(((chunk[i] << i) & 0xFF) | (chunk[i + 1] >> (7 - i)))
    return result.decode()

def is_key_candidate(byte, idx, msg):
    for i in range(idx, len(msg), 32):
        char = chr(byte ^ msg[i])
        if not char.isprintable() and char != '\n':
            return False
    return True

def layer3(payload):
    decoded = base64.a85decode(payload, adobe=True)
    header = '==[ Layer 4/5: '
    key = [decoded[i] ^ ord(header[i]) for i in range(len(header))]
    for i in range(len(key), 32):
        for byte in range(256):
            if is_key_candidate(byte, i, decoded):
                key.append(byte)
                break
    decrypted = bytes(x[0] ^ x[1] for x in zip(decoded, itertools.cycle(key)))
    return decrypted.decode()


def add_with_carry(x, y, c):
    result = x + y + c
    return result & 0xffff, result >> 16

class Packet:
    def __init__(self, source):
        self.data = source

    @property
    def total_size(self):
        return int.from_bytes(self.data[2:4], byteorder='big')

    @property
    def payload(self):
        return self.data[28:self.total_size]

    @property
    def source(self):
        return ipaddress.ip_address(self.data[12:16]), int.from_bytes(self.data[20:22], byteorder='big')

    @property
    def destination(self):
        return ipaddress.ip_address(self.data[16:20]), int.from_bytes(self.data[22:24], byteorder='big')

    def _ipv4_checksum_valid(self):
        total, carry = 0, 0
        for i in range(0, 20, 2):
            total, carry = add_with_carry(int.from_bytes(self.data[i:i+2], byteorder='big'), total, carry)
        return total + carry == 0xffff

    def _udp_checksum_valid(self):
        total, carry = 0, 0
        for i in range(12, 20, 2): # source and destination address
            total, carry = add_with_carry(int.from_bytes(self.data[i:i+2], byteorder='big'), total, carry)
        total, carry = add_with_carry(self.data[9], total, carry) # protocol
        udp_length = int.from_bytes(self.data[24:26], byteorder='big')
        total, carry = add_with_carry(udp_length, total, carry)
        udp_length_mod_2 = udp_length % 2
        for i in range(20, self.total_size - udp_length_mod_2, 2):
            total, carry = add_with_carry(int.from_bytes(self.data[i:i+2], byteorder='big'), total, carry)
        if udp_length_mod_2 == 1:
            total, carry = add_with_carry(self.data[self.total_size - 1] << 8, total, carry)
        return total + carry == 0xffff

    def checksums_valid(self):
        return self._ipv4_checksum_valid() and self._udp_checksum_valid()

def packets(source):
    index = 0
    while index < len(source):
        packet = Packet(source[index:])
        yield packet
        index += packet.total_size

def packet_valid(packet):
    ip_from = ipaddress.ip_address('10.1.1.10')
    destination = ipaddress.ip_address('10.1.1.200'), 42069
    return packet.checksums_valid() and packet.source[0] == ip_from and packet.destination == destination

def layer4(payload):
    decoded = base64.a85decode(payload, adobe=True)
    result = bytearray()
    for packet in filter(packet_valid, packets(decoded)):
        result.extend(packet.payload)
    return result.decode()

def layer5(payload):
    decoded = base64.a85decode(payload, adobe=True)
    key = aes_unwrap_key(decoded[:32], decoded[40:80], int.from_bytes(decoded[32:40], byteorder='big'))
    cipher = AES.new(key, AES.MODE_CBC, iv=decoded[80:96])
    data = cipher.decrypt(decoded[96:])
    return data.decode()

if __name__ == '__main__':
    with open('layer0.txt', 'r') as f:
        payload = get_payload(f.readlines())
    layer = layer0(payload)
    with open('layer1.txt', 'w') as f:
        f.write(layer)
    payload = get_payload(layer.splitlines(keepends=True))
    layer = layer1(payload)
    with open('layer2.txt', 'w') as f:
        f.write(layer)
    payload = get_payload(layer.splitlines(keepends=True))
    layer = layer2(payload)
    with open('layer3.txt', 'w') as f:
        f.write(layer)
    payload = get_payload(layer.splitlines(keepends=True))
    layer = layer3(payload)
    with open('layer4.txt', 'w') as f:
        f.write(layer)
    payload = get_payload(layer.splitlines(keepends=True))
    layer = layer4(payload)
    with open('layer5.txt', 'w') as f:
        f.write(layer)
    payload = get_payload(layer.splitlines(keepends=True))
    layer = layer5(payload)
    with open('layer6.txt', 'w') as f:
        f.write(layer)
