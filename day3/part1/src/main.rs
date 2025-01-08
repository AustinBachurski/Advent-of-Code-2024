use std::fs;
use regex::Regex;


fn main() {
    let input = fs::read_to_string("../../data/day3/input.txt")
        .expect("failed to open input file");

    let pattern = Regex::new(r"mul\(([0-9][0-9]?[0-9]?,[0-9][0-9]?[0-9]?)\)").unwrap();

    let matches = pattern.captures_iter(&input);

    let mut total: i32 = 0;

    for each in matches {
        total += each[1].split(',')
            .map(|x| x.parse().unwrap())
            .collect::<Vec<i32>>().into_iter().product::<i32>();
    }

    println!("{}", total);
}

