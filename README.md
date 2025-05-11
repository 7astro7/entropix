# Entropix: Entropy Analysis for Security and Forensics

Entropix is an entropy analysis tool that identifies encrypted, packed, or anomalous data using Shannon entropy. Designed with security and forensics professionals in mind, it helps detect suspicious files, embedded payloads, and hidden data patterns.

---

## Key Features

- **Shannon entropy calculation** (0.0 to 8.0 bits/byte)
- **Byte frequency histogram**
- **Compression ratio estimation**
- **Per-block entropy scanning** (e.g., 512B chunks)
- **Recursive directory scanning**
- **Flagging of high-entropy files** (potential encryption or obfuscation)
- **JSON output for integration into pipelines**

---

## Security and Forensics Use Cases

### Detect Encrypted or Packed Files
Flag files with entropy above a threshold (default: 7.9):
```bash
./entropix_cli suspicious_payload.bin --entropy-threshold 7.9 --output results.json
```

### Block-wise Scanning
Reveal suspicious regions in larger binary files:
```bash
./entropix_cli disk_image.img --block-scan 512
```

### Recursive Directory Analysis
Triage large directories and flag files for inspection:
```bash
./entropix_cli ~/Downloads --recursive -e 6.5
```

### Identify Stego or Obfuscated Content
Search for hidden data based on entropy anomalies:
```bash
./entropix_cli suspect.jpg --block-scan 1024
```

---

## Output Options

- `--json`: structured output for automation
- `--block-scan N`: report entropy per N-byte block
- `--entropy-threshold X`: customize flagging threshold
- `--extension E`: filter by file extension / file suffix 

---

## Build and Run
```bash
mkdir build && cd build
cmake ..
make
./entropix_cli [file|dir] [flags]
```
