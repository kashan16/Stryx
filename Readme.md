# Stryx - A Modern, Strictly-Typed Programming Language

Stryx is a modern, general-purpose programming language designed to simplify C++ complexities while maintaining strict type safety, expressiveness, and developer productivity. It features an intuitive syntax with modern programming constructs and strong error handling.

## Features

- **Strict Type Safety**: Ensures all variables have predefined types
- **Modern Syntax**: Clean, readable syntax with familiar programming constructs
- **Control Structures**: Comprehensive support for `if/else`, `while`, `for`, and `match` statements
- **Function Definitions**: First-class function support with parameters and return values
- **Pattern Matching**: Advanced `match` statement for expressive control flow
- **Lexical Analysis**: Robust tokenization with support for numbers, strings, and identifiers
- **Abstract Syntax Tree**: Well-structured AST for program representation

## Project Structure

```
Stryx/
├── include/
│   ├── AST.h          # Abstract Syntax Tree definitions
│   └── Token.h        # Token types and structures
├── src/
│   ├── Lexer.cpp      # Lexical analysis implementation
│   ├── Lexer.h        # Lexer class definition
│   ├── Parser.cpp     # Syntax parsing implementation
│   ├── Parser.h       # Parser class definition
│   ├── Token.cpp      # Token implementation
│   └── AST.cpp        # AST node implementations
├── test/
│   └── main.cpp       # Test runner and file reader
├── CMakeLists.txt     # Build configuration
├── README.md          # Project documentation
└── LICENSE            # MIT License
```

## Language Syntax Overview

### Variables and Assignment
```stryx
let x = 10;
var y = 20;
x = 30;  // Assignment
```

### Functions
```stryx
fn add(a, b) {
    return a + b;
}

fn main() {
    let result = add(5, 3);
}
```

### Control Structures
```stryx
// If statements
if (condition) {
    // then branch
} else {
    // else branch
}

// While loops
while (condition) {
    // loop body
}

// For loops
for item in iterable {
    // loop body
}

// Pattern matching
match value {
    1 => { print("One"); }
    2 => { print("Two"); }
    _ => { print("Other"); }
}
```

### Expressions
```stryx
let result = (a + b) * c;
let comparison = x > y && z <= 10;
```

## Getting Started

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- CMake 3.15+

### Building from Source

```bash
# Clone the repository
git clone https://github.com/kashan16/Stryx.git
cd Stryx

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make
```

### Running the Lexer

```bash
# Run the lexer on a Stryx source file
./stryx_lexer examples/hello_world.styx
```

### Example Program

Create a file `hello_world.styx`:

```stryx
fn main() {
    let message = "Hello, Stryx!";
    print(message);
}
```

## Current Implementation Status

### ✅ Implemented Features

**Lexer:**
- Tokenization of identifiers, numbers (integer and float), strings
- Operator recognition (+, -, *, /, %, ==, !=, <, <=, >, >=, =, =>)
- Keyword recognition (fn, let, var, return, if, else, for, while, match)
- Comment support (single-line and multi-line)
- Whitespace and newline handling

**Parser:**
- Function declaration parsing
- Variable declarations (let, var) and assignments
- Control structures (if/else, while, for, match)
- Binary expressions with operator precedence
- Pattern matching with multiple arms
- Function calls with arguments

**AST:**
- Comprehensive node types for expressions and statements
- Support for complex control flow structures
- Extensible design for future language features

### 🚧 In Progress

- Semantic analysis and type checking
- Code generation backend
- Standard library development
- Memory management system

## Development Roadmap

### Phase 1: Core Language (Current)
- [x] Lexical analysis
- [x] Syntax parsing
- [x] Abstract Syntax Tree
- [ ] Semantic analysis
- [ ] Basic type system
- [ ] Simple code generation

### Phase 2: Language Features
- [ ] Type annotations and inference
- [ ] Structures and enumerations
- [ ] Generic programming
- [ ] Error handling mechanisms
- [ ] Module system
- [ ] Standard library

### Phase 3: Advanced Features
- [ ] Memory safety guarantees
- [ ] Concurrency primitives
- [ ] Metaprogramming capabilities
- [ ] Package manager
- [ ] Debugging support

### Phase 4: Production Ready
- [ ] Optimization passes
- [ ] Cross-compilation support
- [ ] Language server protocol
- [ ] Comprehensive documentation
- [ ] Performance benchmarking

## Contributing

We welcome contributions from the community! Here's how you can help:

### Reporting Issues
- Use the issue template to report bugs
- Include reproducible examples
- Specify your environment details

### Feature Requests
- Describe the use case clearly
- Consider if it aligns with Stryx's design goals
- Discuss implementation approach

### Code Contributions
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

### Development Guidelines
- Follow the existing code style
- Write tests for new features
- Update documentation accordingly
- Ensure backward compatibility

## Language Design Principles

1. **Simplicity**: Easy to learn and use
2. **Expressiveness**: Write clear, concise code
3. **Safety**: Prevent common programming errors
4. **Performance**: Generate efficient native code
5. **Interoperability**: Work well with existing C/C++ codebases

## Building and Testing

### Running Tests
```bash
cd build
make test
```

### Code Formatting
We use clang-format for consistent code style:
```bash
find src include test -name '*.h' -o -name '*.cpp' | xargs clang-format -i
```

### Debug Build
```bash
mkdir build-debug && cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by modern languages like Rust, Swift, and TypeScript
- Built with a focus on developer experience and code safety
- Thanks to the open-source community for invaluable tools and libraries

---

<div align="center">
  <strong>Built with passion by Mohd Kashan Yunus</strong>
  <br>
  <em>Making programming safer and more expressive</em>
</div>
