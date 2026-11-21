# Forensic JPEG Data Recovery & Bitwise Memory Analysis

A low-level digital forensics utility written in C designed to extract and reconstruct deleted JPEG images from raw binary memory card dumps (`card.raw`).

## 🔬 Memory Architecture & Bitwise Detection

File systems like FAT32 store data in contiguous **512-byte blocks**. When files are deleted, their underlying binary data remains intact until overwritten.

- **Buffer Streaming:** Reads raw binary streams in deterministic 512-byte chunks (`uint8_t buffer[512]`) via low-level File I/O (`fread` / `fwrite`).
- **Bitwise Signature Masking:** Identifies the start of a new JPEG file by verifying the first three magic bytes (`0xff 0xd8 0xff`) and applying a bitwise `AND` mask (`(buffer[3] & 0xf0) == 0xe0`) to validate the fourth header byte's upper nibble.
- **Zero-Leak File Management:** Dynamically allocates sequential output files (`000.jpg`, `001.jpg`, ...) while strictly managing file pointers (`fclose`) to eliminate memory leaks.

## 🚀 Execution & Setup

```bash
# Compile the forensic recovery tool
make recover

# Recover JPEG files from a raw memory image
./recover card.raw
```

