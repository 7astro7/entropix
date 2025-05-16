
[![Build](https://github.com/7astro7/entropix/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/7astro7/entropix/actions/workflows/build_and_test.yml)

# Entropix: Entropy Analysis for Security and Forensics

Entropix is a CLI tool that analyzes Shannon entropy to flag files and blocks with unusually high entropy. Designed with security and forensics professionals in mind, it helps identify potentially encrypted, obfuscated, or anomalous data by detecting entropy spikes in files and directories.

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
Flag files with entropy above a threshold (7.9 in this example):
```bash
./entropix_cli suspicious_payload.bin --entropy-threshold 7.9 --output results.json
```

### Block-wise Scanning
Reveal high entropy regions within larger files:
```bash
./entropix_cli disk_image.img --block-scan 512
```

### Recursive Directory Analysis
Triage large directories and flag files for inspection:
```bash
./entropix_cli ~/Downloads --recursive -et 6.5
```

## Example JSON output
```
$ ./entropix_cli /path/to/scan -o results.json -et 7.0
Report written to results.json: 

[
  {
    "entropy": 6.337943418957542,
    "path": "scanned_file",
    "threshold": 6.0,
    "type": "global"
  }
]

$ ./entropix_cli /path/to/scan -o results.json -et 4 
Report written to results.json: 
....
      {
        "entropy": 4.936071280649427,
        "offset": 3085824
      },
      {
        "entropy": 4.942376652266078,
        "offset": 3086336
      },
      {
        "entropy": 5.026694806386195,
        "offset": 3086848
      }
    ],
    "file_entropy": 6.337943418957542,
    "path": "scanned_file",
    "threshold": 4.0,
    "type": "block"
  }
]
```

## Command-Line Options
```
Usage:
    entropix_cli <path> [options]

Arguments:
    <path>                     File or directory to scan

Options:
    --entropy-threshold, -et <value>
                               Flag files with entropy above this value (default: 7.9)
    --block-scan, -b <size>    Perform block-wise entropy scan (e.g. 512 for 512B chunks)
    --recursive, -r            Recursively scan subdirectories
    --extension, -e <.ext>     Only include files with the given extension (e.g. .bin)
    --output, -o <file>        Write JSON report to file
    --verbose, -v              Print per-file entropy to stdout
    --help                     Show this message
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

## Testing
Entropix uses Google Test for unit testing.

To build and run the test suite:

```
mkdir build && cd build
cmake -DBUILD_TESTING=ON ..
make
ctest
```

## Contributing 
Contributions are highly encouraged. If you would like to contribute, you can:
- Fork the repository and submit pull requests with your improvements, bug fixes, test additions, or new features.
- Report any issues or bugs you encounter on the issue tracker.
- Help improve the documentation by submitting documentation improvements or corrections.

