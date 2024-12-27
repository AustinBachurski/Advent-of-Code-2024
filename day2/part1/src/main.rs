use std::fs;
use std::cmp::Ordering;


fn is_safe(report: &Vec<i32>) -> bool {
    if report.len() < 2 {
        return false;
    }

    let mut compare: Ordering = Ordering::Equal;

    let mut values = report.iter();

    let mut current = values
        .next()
        .expect("current value did not exist");

    while let Some(value) = values.next(){
        let comparison = current.cmp(&value);

        if comparison == Ordering::Equal {
            return false;
        }

        if (current - value).abs() > 3 {
            return false;
        }

        if compare == Ordering::Equal {
            compare = comparison;
            current = value;
            continue;
        }

        if compare != comparison {
            return false;
        }

        current = value;
    }

    return true;
}

fn main() {
    let input = fs::read_to_string("../../data/day2/input.txt")
        .expect("failed to open input file");

    let reports: Vec<Vec<i32>> = input
        .split('\n')
        .map(|report| report.split_whitespace()
            .map(|x| x.parse::<i32>()
                .expect("x did not convert to i32"))
            .collect())
        .collect();

    let mut safe: usize = 0;

    for report in reports {
       if is_safe(&report) { safe += 1 } 
    }

    println!("{}", safe);
}

