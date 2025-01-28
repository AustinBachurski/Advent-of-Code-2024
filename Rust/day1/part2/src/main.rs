use std::fs;


fn main() {
    let input = fs::read_to_string("../../data/day1.txt")
        .expect("failed to open input file");

    let mut values = input.split_whitespace();

    let mut left: Vec<usize> = vec![];
    let mut right: Vec<usize> = vec![];

    while let Some(value) = values.next() {
        left.push(value
            .parse()
            .expect("value did not convert to usize"));

        right.push(values
            .next()
            .expect("value did not exist")
            .parse()
            .expect("value did not convert to usize"));
    }

    let mut count: usize;

    for i in 0..left.len() {
        count = 0;
        for value in &right {
            if *value == left[i] {
                count += 1;
            }
        }
        left[i] = count * left[i];
    }

    let result: usize = left.iter().sum();

    println!("{}", result);
}

