use std::fs;


fn main() {
    let input = fs::read_to_string("../../data/day5/input.txt")
        .expect("failed to open input file");

    let ordering: Vec<(&str, &str)> = input
        .split("\r\n")
        .filter(|s| s.contains('|'))
        .map(|x| {
            let mut i = x.split('|'); 
            (i.next().unwrap(), i.next().unwrap())
        })
        .collect();

    let updates: Vec<Vec<&str>> = input
        .split("\r\n")
        .filter(|s| s.contains(','))
        .map(|s| {
            s.split(',').collect()
        })
        .collect();

    let mut buffer: Vec<&str>;
    let mut swapped;
    let mut changes = true;
    let mut order: Vec<&str>;

    let mut current_index: Option<usize>;
    let mut order_index: Option<usize>;

    let mut result: usize = 0;

    for update in updates {

        buffer = update.clone();

        while changes {
            changes = false;
            for value in &update {
                order = ordering.iter()
                    .filter(|s| s.0 == *value)
                    .map(|s| s.1)
                    .collect();

                for position in &order {
                    swapped = true;
                    while swapped {
                        current_index = buffer.iter().position(|x| x == value);
                        order_index = buffer.iter().position(|x| x == position);

                        if current_index.is_some() && order_index.is_some()
                        && current_index.unwrap() > order_index.unwrap() {
                            buffer.swap(current_index.unwrap(), order_index.unwrap());
                            changes = true;
                        } else {
                            swapped = false;
                        }

                    }
                }
            }
        }
        changes = true;

        if buffer != update {
            result += buffer[buffer.len() / 2].parse::<usize>().unwrap();
        }

    }

    println!("{}", result);

}

