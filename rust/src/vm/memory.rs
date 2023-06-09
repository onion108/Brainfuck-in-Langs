#[derive(Clone, Debug)]
pub struct Memory {
    positive_space: Vec<i32>,
    negative_space: Vec<i32>,
}

fn zeros(count: usize) -> Vec<i32> {
    let mut result = Vec::new();
    for _ in 0..count {
        result.push(0);
    }
    result
}

impl Memory {
    pub fn new() -> Self {
        Self {
            positive_space: zeros(256), 
            negative_space: zeros(256),
        }
    }
    fn assume_slot_possible(&mut self, idx: i32) {
        if idx >= 0 {
            while idx >= self.positive_space.len() as i32 {
                self.positive_space.push(0);
            }
        } else {
            let translated_idx = -idx-1;
            while translated_idx >= self.negative_space.len() as i32 {
                self.negative_space.push(0);
            }
        }
    }
    pub fn set(&mut self, idx: i32, value: i32) {
        self.assume_slot_possible(idx);
        if idx >= 0 {
            self.positive_space[idx as usize] = value;
        } else {
            self.negative_space[(-idx-1) as usize] = value;
        }
    }
    pub fn get(&mut self, idx: i32) -> i32 {
        self.assume_slot_possible(idx);
        if idx >= 0 {
            self.positive_space[idx as usize]
        } else {
            self.negative_space[(-idx-1) as usize]
        }
    }
}

