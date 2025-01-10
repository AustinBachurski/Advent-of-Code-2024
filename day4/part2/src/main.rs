use std::fs;
use std::collections::HashSet;

#[derive(PartialEq, PartialOrd, Eq, Ord, Hash)]
struct Index {
    row: usize,
    column: usize,
}

#[derive(PartialEq, PartialOrd, Eq, Ord, Hash)]
struct WordIndicies {
    one: Index,
    two: Index,
    three: Index,
    four: Index,
    five: Index,
}

fn is_festive(indicies: &WordIndicies, lines: &Vec<&str>) -> bool {
    (
        (
        lines[indicies.one.row].chars().nth(indicies.one.column) == Some('M')
        && lines[indicies.two.row].chars().nth(indicies.two.column) == Some('A')
        && lines[indicies.three.row].chars().nth(indicies.three.column) == Some('S')
        )
        ||
        (
        lines[indicies.three.row].chars().nth(indicies.three.column) == Some('M')
        && lines[indicies.two.row].chars().nth(indicies.two.column) == Some('A')
        && lines[indicies.one.row].chars().nth(indicies.one.column) == Some('S')
        )
    )
    &&
    (
        (
        lines[indicies.four.row].chars().nth(indicies.four.column) == Some('M')
        && lines[indicies.two.row].chars().nth(indicies.two.column) == Some('A')
        && lines[indicies.five.row].chars().nth(indicies.five.column) == Some('S')
        )
        ||
        (
        lines[indicies.five.row].chars().nth(indicies.five.column) == Some('M')
        && lines[indicies.two.row].chars().nth(indicies.two.column) == Some('A')
        && lines[indicies.four.row].chars().nth(indicies.four.column) == Some('S')
        )
    )
}


fn main() {
    let input = fs::read_to_string("../../data/day4.txt")
        .expect("failed to open input file");

    let lines: Vec<&str> = input.split('\n').collect();

    let row_bounds = lines.len() - 4;
    let column_bounds = lines[0].len() - 4;

    let mut row: usize = 0;
    let mut column: usize = 0;

    let mut count: HashSet<WordIndicies> = HashSet::new();

    while row <= row_bounds {
        while column <= column_bounds {
            let indicies = WordIndicies { 
                one: Index { row, column },
                two: Index { row: row + 1, column: column + 1 },
                three: Index { row: row + 2, column: column + 2 },
                four: Index { row: row, column: column + 2 },
                five: Index { row: row + 2, column: column } ,
            };

            if is_festive(&indicies, &lines) {
                count.insert(indicies);
            }
            column += 1;
        }
        row += 1;
        column = 0;
    }

    println!("{}", count.len());
}

