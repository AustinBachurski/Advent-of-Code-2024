use std::fs;


struct Pathing {
    position: isize,
    direction: isize,
    up: isize,
    right: isize,
    down: isize,
    left: isize,
}

impl Pathing {
    pub fn new(guard: (usize, char), line_length: usize) -> Self {
        Self {
            position: guard.0 as isize,
            direction: match guard.1 {
                '^' => -(line_length as isize),
                '>' => -1,
                'v' => line_length as isize,
                '<' => 1,
                _ => panic!()
            },
            up: -(line_length as isize),
            right: 1,
            down: line_length as isize,
            left: -1,
        }
    }

    pub fn turn(&mut self) {
        self.position -= self.direction;

        self.direction = match self.direction {
            d if d == self.up => self.right,
            d if d == self.right => self.down,
            d if d == self.down => self.left,
            d if d == self.left => self.up,
            _ => panic!(),
        }
    }

    pub fn next(&mut self) {
        self.position += self.direction;
    }
}

pub fn find_guard(input: &str) -> (usize, char) {
    let guard = if let Some(up) = input.find('^') {
        (up, '^')
    } else if let Some(right) = input.find('>') {
        (right, '>')
    } else if let Some(down) = input.find('v') {
        (down, 'v')
    } else if let Some(left) = input.find('<') {
        (left, '<')
    } else {
        panic!();
    };
    guard
}

fn main() {
    let mut input = fs::read_to_string("../../data/day6.txt")
        .expect("failed to open input file");

    let mut path = Pathing::new(find_guard(&input), input.find('\n').unwrap() + 1);

    loop {
        input.replace_range(path.position as usize..=path.position as usize, "X");
        path.next();

        match input.chars().nth(path.position as usize) {
            Some('#') => path.turn(),
            Some('\r') => break,
            None => break,
            _ => continue,
        }
    }

    println!("{}", input.chars().filter(|c| *c == 'X').count());

}

