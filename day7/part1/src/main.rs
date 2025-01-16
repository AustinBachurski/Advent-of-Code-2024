use std::fs;


fn main() {
    let input = fs::read_to_string("../../data/day7.txt").expect("Failed to open input file.");

    let targets: Vec<u128> = input
        .split_whitespace()
        .filter(|s| s.contains(':'))
        .map(|s| s.split_once(':').unwrap().0.parse().unwrap())
        .collect();

    let mut data: Vec<(u128, Vec<u128>)> = Vec::new();

    for (index, line) in input.lines().enumerate() {
        data.push(
            (targets[index], line
                .split_whitespace()
                .filter_map(|x| x.parse::<u128>().ok())
                .collect())
        );
    }

    let mut result: u128 = 0;
    let mut value: u128;
    let mut op_count: usize;
    let mut shift: usize;

    'evaluation: for each in data {
        op_count = each.1.len() - 1;

        for op in 0..=(u16::MAX >> (16 - op_count - 1)) {
            let mut iter = each.1.iter();
            value = *iter.next().expect("First element has to be there.");
            shift = op_count;

            while let Some(number) = iter.next() {
                if (op & (1 << shift)) == 0 {
                    value += number;
                } else {
                    value *= number;
                }
                shift -= 1;
            }
            if value == each.0 {
                result += each.0;
                continue 'evaluation;
            }
        }
    }

    println!("{}", result);

}

