# C++ ST_STL (Stulu Standart Template Library)

## Overview

This project started as an educational endeavor to learn and understand the C++ Standard Template Library (STL) and expand my C++ skills. Now, I aim to build on this foundation by adding include-only libraries that will assist day-to-day programmers. These additions will cover various functionalities including networking, extended storage types, windowing, graphics, input handling, and more.

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Usage](#usage)
6. [License](#license)

## Introduction

Initially created for educational purposes, this project serves as a comprehensive resource for programmers looking to enhance their C++ skills by utilizing and extending the STL. The goal is to provide a robust set of include-only libraries that simplify common programming tasks.

## Features

- **Networking:** Simplified classes and functions for network programming.
- **Extended Storage Types:** Additional data structures and storage solutions. (WIP)
- **Windowing:** Tools for creating and managing application windows. (WIP)
- **Graphics:** Basic graphics library for rendering shapes, images, and text. (WIP)
- **Input Handling:** Utilities for capturing and processing user input. (WIP)

## Installation

To use these libraries in your project, simply clone the repository and include the desired headers in your project files.

```bash
git clone https://github.com/stulu08/st_stl.git
```

Include the necessary headers in your C++ source files:

```cpp
#include <stulu/stulu.h>
```

## Usage

### Networking Example

```cpp
#include <stulu/networking.h>

namespace net = stulu::networking;

int main() {
  net::SSA::Startup();
  net::socket socket;

  int status = socket.connect("github.com", "80");
  if (status != 0) {
    printf("Failed connecting: %d", net::SSA::GetLastError());
    return -1;
  }
  int bytesSent = socket.send("GET / HTTP/1.1\r\n\r\n");
  if (bytesSent == 0) {
    printf("Failed sending: %d", net::SSA::GetLastError());
    return -1;
  }

  stulu::string response = socket.receive();
  printf("Received answer (%d bytes):\n%s", (int)response.size(), response.c_str());
  return 0;
}
```
## License

This project is licensed under the BSD-3-Clause License - see the [LICENSE](LICENSE) file for details.

---
