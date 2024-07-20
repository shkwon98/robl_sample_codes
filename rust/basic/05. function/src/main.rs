fn main() {
    function_0();
    function_1(5);

    let x = 5;
    let y = 6;
    let sum = function_2(x, y);
    println!("The sum of {x} and {y} is: {sum}");
}

fn function_0() {
    println!("Another function.");
}

fn function_1(x: i32) {
    println!("The value of x is: {x}");
}

fn function_2(x: i32, y: i32) -> i32 {
    println!("The value of x is: {x}");
    println!("The value of y is: {y}");

    x + y
}
