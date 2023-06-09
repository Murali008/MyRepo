#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PACKET_DATA_LENGTH (50)

typedef struct data_packet_t {
    uint8_t id;
    uint8_t data_length;
    uint8_t data[MAX_PACKET_DATA_LENGTH];
    uint16_t crc;
} data_packet_t;

// CRC calculation function
uint16_t calculate_crc(const data_packet_t* packet) {
    uint16_t crc = 0xFFFF;  // Initialize CRC to 0xFFFF
    
    // Perform CRC calculation on the packet data
    for (int i = 0; i < packet->data_length; i++) {
        crc ^= packet->data[i];
        
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) {
                crc >>= 1;
                crc ^= 0xA001;
            } else {
                crc >>= 1;
            }
        }
    }
    
    return crc;
}


bool is_packet_corrupted(const data_packet_t* packet) {
    uint16_t calculated_crc = calculate_crc(packet);
    return packet->crc != calculated_crc;
}


int main() {
    // Create a sample data packet
    data_packet_t packet;
    packet.id = 1;
    packet.data_length = 5;
    packet.data[0] = 'H';
    packet.data[1] = 'e';
    packet.data[2] = 'l';
    packet.data[3] = 'l';
    packet.data[4] = 'o';
    
    // Calculate CRC for the packet
    packet.crc = calculate_crc(&packet);
    
    // Print the packet information
    printf("Packet ID: %u\n", packet.id);
    printf("Data Length: %u\n", packet.data_length);
    printf("Data: ");
    for (int i = 0; i < packet.data_length; i++) {
        printf("%c ", packet.data[i]);
    }
    printf("\nCRC: 0x%04X\n", packet.crc);
    
    // Check if the packet is corrupted
    int isCorrupted = is_packet_corrupted(&packet);
    if (isCorrupted) {
        printf("Packet is corrupted.\n");
    } else {
        printf("Packet is not corrupted.\n");
    }
    
    return 0;
}
