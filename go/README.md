# [Go](https://golang.org/)


## Table of Contents
- [Go](#go)
  - [Table of Contents](#table-of-contents)
    - [How to Install Go on Linux](#how-to-install-go-on-linux)
    - [How to Start a New Go Project](#how-to-start-a-new-go-project)
    - [How to Install Dependencies(Go Packages) in a Go Project](#how-to-install-dependenciesgo-packages-in-a-go-project)


### How to Install Go on Linux
1. Download the latest version of Go from the official [Go website](https://golang.org/dl/)

1. Remove any previous Go installation by deleting the /usr/local/go folder (if it exists)
    ```bash
    sudo rm -rf /usr/local/go
    ```

1. Extract the archive you just downloaded into /usr/local, creating a fresh Go tree in /usr/local/go
    ```bash
    sudo tar -C /usr/local -xzf go1.22.5.linux-amd64.tar.gz
    ```

1. Add /usr/local/go/bin to the PATH environment variable
    ```bash
    export PATH=$PATH:/usr/local/go/bin
    ```

1. Verify that you've installed Go by opening a command prompt and typing the following command
    ```bash
    go version
    ```


### How to Start a New Go Project
1. Create a new directory for your project
    ```bash
    mkdir my-go-project
    cd my-go-project
    ```

1. Create a new Go module
    ```bash
    go mod init github.com/username/my-go-project
    ```

1. Create a new Go file
    ```bash
    touch main.go
    ```

1. Write some Go code in main.go

1. Build your Go program
    ```bash
    go build
    ```

1. Or Directly run your Go program
    ```bash
    go run main.go
    ```


### How to Install Dependencies(Go Packages) in a Go Project
1. Install a dependency using the go get command
   For example, to install the gin-gonic/gin package, you can run the following command
    ```bash
    go get github.com/gin-gonic/gin
    ```

1. Import the package in your Go code
    ```go
    import "github.com/gin-gonic/gin"
    ```
