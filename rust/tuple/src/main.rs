fn main() {
    let mut tup: (i32, bool, char) = (777, true, 'h');
    println!("Tuple: {:?}", tup);

    tup.2 = 'a';
    println!("Tuple: {:?}", tup);
}
