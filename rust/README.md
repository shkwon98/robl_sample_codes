# Rust


### How to Install [Rust](https://www.rust-lang.org/) on Linux
1. Download and run the official Rust installer rustup, using the following command
    ```bash
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
    ```

1. Follow the on-screen instructions to complete the installation

1. Verify that you've installed Rust by opening a command prompt and typing the following command
    ```bash
    rustc --version
    ```


### How to Start a New Rust Project
1. Create a new rust project using Cargo
    ```bash
    cargo new my-rust-project
    cd my-rust-project
    ```

1. Write some Rust code in the src/main.rs file

1. Build your Rust project
    ```bash
    cargo build
    ```

1. Or Directly run your Rust project
    ```bash
    cargo run
    ```


### How to Install Dependencies(Crates) in a Rust Project
1. Add a dependency to your Cargo.toml file
    ```toml
    [dependencies]
    rand = "0.8.4"
    ```

1. Build your project to download and compile the dependencies
    ```bash
    cargo build
    ```

1. Use the dependency in your Rust code
    ```rust
    use rand::Rng;
    ```

1. Build and run your project to apply the changes
    ```bash
    cargo run
    ```
