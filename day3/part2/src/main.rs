use std::fs;
use regex::Regex;


fn main() {
    let mut input = fs::read_to_string("../../data/day3/input.txt")
        .expect("failed to open input file");

    let mut start = input.find("don't()");
    let mut end = input.find("do()");

    while start.is_some() {
        while end.is_some() {
            if end.unwrap() < start.unwrap() {
                input.replace_range(end.unwrap()..=end.unwrap(), " ");
                end = input.find("do()");
            } else {
                break;
            }
        }

        if end.is_some() {
            input.replace_range(start.unwrap()..=end.unwrap(), " ");
        } else {
            input.replace_range(start.unwrap()..input.len(), " ");
        }

        start = input.find("don't()");
        end = input.find("do()");
    }

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

