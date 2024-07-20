fn main() {
    let mut i = 0;

    loop {
        if i == 10 {
            break;
        }

        println!("Hello, i!");
        i += 1;
    }

    let mut j = 0;

    while j < 10 {
        println!("Hello, j!");
        j += 1;
    }

    for _k in 0..10 {
        println!("Hello, k!");
    }
}
