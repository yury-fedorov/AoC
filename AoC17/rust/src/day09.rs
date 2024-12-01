pub fn task1(text: &str) -> i32 {
    tasks(text).0
}

pub fn task2(text: &str) -> i32 {
    tasks(text).1
}

pub fn tasks(text: &str) -> (i32, i32) {
    let mut level = 1;
    let mut total_score = 0;
    let mut total_garbage = 0;
    let mut prev_ch = ' ';
    let mut is_garbage = false;
    for ch in text.chars() {
        if prev_ch == '!' && ch == '!' {
            prev_ch = ' ';
        } else {
            if prev_ch != '!' {
                match ch {
                    '{' => {
                        if !is_garbage {
                            total_score += level;
                            level += 1;
                        } else {
                            total_garbage += 1;
                        }
                    }
                    '}' => {
                        if !is_garbage {
                            level -= 1;
                        } else {
                            total_garbage += 1;
                        }
                    }
                    '<' => {
                        if is_garbage {
                            total_garbage += 1;
                        } else {
                            is_garbage = true;
                        }
                    }
                    '>' => {
                        is_garbage = false;
                    }
                    '!' => {}
                    _ => {
                        if is_garbage {
                            total_garbage += 1;
                        }
                    }
                }
            }
            prev_ch = ch;
        }
    }
    (total_score, total_garbage)
}
