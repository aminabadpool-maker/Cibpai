# --- Stage 1: Build stage ---
FROM alpine:3.19 AS builder

# Install build essentials, Clang, FFmpeg dev, and Libcurl dev
RUN apk add --no-cache \
    clang \
    make \
    ffmpeg-dev \
    curl-dev \
    build-base

WORKDIR /app
COPY . .

# Compile with high optimization
RUN make clean && make -j$(nproc)

# --- Stage 2: Runtime stage ---
FROM alpine:3.19

# Install runtime dependencies only
RUN apk add --no-cache \
    ffmpeg \
    curl \
    ca-certificates

WORKDIR /app

# Copy the compiled binary from builder stage
COPY --from=builder /app/cibpai /usr/local/bin/cibpai

# Set execution entrypoint
ENTRYPOINT ["cibpai"]
