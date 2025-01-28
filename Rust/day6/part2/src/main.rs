use std::fs;
use std::thread;
use std::sync::{Arc, Mutex};
use std::thread::JoinHandle;


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
    let input = fs::read_to_string("../../data/day6.txt")
        .expect("failed to open input file");

    let mut board = input.clone();

    let mut path = Pathing::new(find_guard(&board), board.find('\n').unwrap() + 1);

    loop {
        board.replace_range(path.position as usize..=path.position as usize, "X");
        path.next();

        match board.chars().nth(path.position as usize) {
            Some('#') => path.turn(),
            Some('\r') => break,
            None => break,
            _ => continue,
        }
    }

    // Utterly Horrid Brute Force Attempt

    let mut spaces: Vec<usize> = Vec::new();

    for i in 0..board.len() {
        if board.chars().nth(i) == Some('X') {
            spaces.push(i);
        }
    }

    let obstacles: Arc<Mutex<usize>> = Arc::new(Mutex::new(0));

    let mut threads: Vec<JoinHandle<_>> = Vec::new();

    for space in spaces {
        let obstacles = Arc::clone(&obstacles);
        let mut board = input.clone();

        threads.push(thread::spawn( move || {

            let mut path = Pathing::new(find_guard(&board), board.find('\n').unwrap() + 1);


            board.replace_range(space..=space, "#");

            let mut steps: usize = 0;
            const MAX_STEPS:usize = 5000;

            loop {
                path.next();

                match board.chars().nth(path.position as usize) {
                    Some('#') => path.turn(),
                    Some('\r') => break,
                    None => break,
                    _ => continue,
                }
                steps += 1;

                if steps == MAX_STEPS {
                    let mut count = obstacles.lock().unwrap();
                    *count += 1;
                    break;
                }
            }
        }));
    }

    for each in threads {
        let _ = each.join().unwrap();
    }

    let count = obstacles.lock().unwrap();
    println!("Final Answer: {}", count);

}

