fn main() {
    let mut x = 5;
    println!("The value of x is: {x}");
    {
        x = x + 1;
        println!("The value of x is: {x}");
    }
    x = x * 2;
    println!("The value of x is: {x}");

    let y = 5;
    println!("The value of y is: {y}");
    {
        let y = y + 1;
        println!("The value of y is: {y}");
    }
    let y = y * 2;
    println!("The value of y is: {y}");
}
