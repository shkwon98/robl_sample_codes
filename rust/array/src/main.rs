use std::collections::vec_deque;

fn main() {
    let mut arr = [1, 2, 3, 4, 5];
    println!("Array: {:?}", arr);

    arr[2] = 33;
    println!("Array: {:?}", arr);

    // This is not allowed
    // arr = [6, 7, 8, 9, 10, 11];
    // println!("Array: {:?}", arr);
}
