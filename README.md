# Cibpai: C is Best for AI

> **"Cibpai"** is a high-performance, lightweight C11 video AI pipeline designed specifically for edge devices, low-level system architectures, and resource-constrained environments like Termux on ARM64.

---

## 💡 Philosophy & Core Vision

In a software world increasingly dominated by memory-heavy runtimes, interpreted languages, and bloated layers, **Cibpai** stands as a testament to the power of bare-metal efficiency and systems programming.

* **Why C and ARM Assembly?** Because low-level control matters. When processing video streams and interacting with AI endpoints (like Google Gemini), avoiding unnecessary abstractions ensures maximum execution speed, minimal memory footprint, and absolute predictability.
* **The Edge First:** Operating systems and AI pipelines should be transparent, lightning-fast, and fully understandable down to the bare metal. This project proves that modern multimedia ingestion and neural network integration don't require heavy dependencies—just clean C11, FFmpeg, and `libcurl`.

---

## 📁 Project Architecture

```text
Cibpai/
├── include/
│   └── cibpai.h        # Core header definitions and prototypes
├── src/
│   ├── main.c          # Entry point and CLI orchestrator
│   ├── extractor.c     # FFmpeg frame extraction logic
│   └── ai_client.c     # Custom Base64 encoder & JSON payload builder with libcurl
├── Makefile            # Optimized build rules (-O3, clang)
└── install.sh          # Automated toolchain and dependency setup script
```

---

## 🚀 Quick Start & Usage

1. **Run the automated installer:**
   ```bash
   ./install.sh
   ```
2. **Set your Gemini API Key:**
   ```bash
   export AI_API_KEY="your_actual_gemini_api_key"
   ```
3. **Run the pipeline on a video:**
   ```bash
   ./cibpai test.mp4 frames_out
   ```

---

## 📄 License

This project is open-source software released under the **MIT License**.
