use std::fs;


fn main() {
    let input = fs::read_to_string("../../data/day1/input.txt")
        .expect("failed to open input file");

    let mut values = input.split_whitespace();

    let mut left: Vec<i32> = vec![];
    let mut right: Vec<i32> = vec![];

    while let Some(value) = values.next() {
        left.push(value
            .parse()
            .expect("value did not convert to i32"));

        right.push(values
            .next()
            .expect("value did not exist")
            .parse()
            .expect("value did not convert to i32"));
    }

    left.sort();
    right.sort();

    for i in 0..left.len() {
        left[i] = (right[i] - left[i]).abs();
    }

    let result: i32 = left.iter().sum();

    println!("{}", result);
}

