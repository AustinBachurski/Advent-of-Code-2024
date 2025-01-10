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
}

fn is_festive(indicies: &WordIndicies, lines: &Vec<&str>) -> bool {
		(
			lines[indicies.one.row].chars().nth(indicies.one.column) == Some('X')
			&& lines[indicies.two.row].chars().nth(indicies.two.column) == Some('M')
			&& lines[indicies.three.row].chars().nth(indicies.three.column) == Some('A')
			&& lines[indicies.four.row].chars().nth(indicies.four.column) == Some('S')
			)
		||
		// Backwards
		(
			lines[indicies.four.row].chars().nth(indicies.four.column) == Some('X')
			&& lines[indicies.three.row].chars().nth(indicies.three.column) == Some('M')
			&& lines[indicies.two.row].chars().nth(indicies.two.column) == Some('A')
			&& lines[indicies.one.row].chars().nth(indicies.one.column) == Some('S')
			)
}

fn get_indicies(row: usize, column: usize) -> [WordIndicies; 10] {
    [
        // Horizontal
        WordIndicies { 
            one: Index { row, column },
            two: Index{ row, column: column + 1 },
            three: Index { row, column: column + 2 },
            four: Index { row, column: column + 3 } 
        },
        WordIndicies { 
            one: Index { row: row + 1, column },
            two: Index { row: row + 1, column: column + 1 },
            three: Index { row: row + 1, column: column + 2 },
            four: Index { row: row + 1, column: column + 3 }
        },
        WordIndicies { 
            one: Index { row: row + 2, column },
            two: Index { row: row + 2, column: column + 1 },
            three: Index { row: row + 2, column: column + 2 },
            four: Index { row: row + 2, column: column + 3 }
        },
        WordIndicies { 
            one: Index { row: row + 3, column },
            two: Index { row: row + 3, column: column + 1 },
            three: Index { row: row + 3, column: column + 2 },
            four: Index { row: row + 3, column: column + 3 }
        },

        // Vertical
        WordIndicies {
            one: Index{ row, column },
            two: Index { row: row + 1, column },
            three: Index { row: row + 2, column },
            four: Index { row: row + 3, column } },
        WordIndicies {
            one: Index { row, column: column + 1 },
            two: Index { row: row + 1, column: column + 1 },
            three: Index { row: row + 2, column: column + 1 },
            four: Index { row: row + 3, column: column + 1 } },
        WordIndicies {
            one: Index { row, column: column + 2 },
            two: Index { row: row + 1, column: column + 2 },
            three: Index { row: row + 2, column: column + 2 },
            four: Index { row: row + 3, column: column + 2 } },
        WordIndicies { 
            one: Index { row, column: column + 3 },
            two: Index { row: row + 1, column: column + 3 },
            three: Index { row: row + 2, column: column + 3 },
            four: Index { row: row + 3, column: column + 3 } },

        // Diaganal
        WordIndicies {
            one: Index { row, column },
            two: Index { row: row + 1, column: column + 1 },
            three: Index { row: row + 2, column: column + 2 },
            four: Index { row: row + 3, column: column + 3 } },
        WordIndicies {
            one: Index { row, column: column + 3 },
            two: Index { row: row + 1, column: column + 2 },
            three: Index { row: row + 2, column: column + 1 },
            four: Index { row: row + 3, column } },
    ]
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
            let indicies = get_indicies(row, column);

            for each in indicies {
                if is_festive(&each, &lines) {
                    count.insert(each);
                }
            }
            column += 1;
        }
        row += 1;
        column = 0;
    }

    println!("{}", count.len());
}

