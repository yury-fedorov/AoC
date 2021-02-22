use super::{Instruction, Register, Registers, Value, ValueArgument, Argument, RegisterArgument, parse_instruction};
use super::Instruction::*;
use std::sync::mpsc::{Sender, Receiver, channel};
use crate::day18david::task2::Message::ValueMessage;

enum Message {
    ValueMessage(Value),
    Blocked{received_messages_count: usize}
}

enum StepResult {
    Continue,
    End
}

use Message::*;
use StepResult::*;
use crate::common;
use std::thread;

pub(super) struct Duet {
    instructions: Vec<Instruction>,
    registers: Registers,
    instruction_pointer: usize,

    sender: Sender<Message>,
    receiver: Receiver<Message>,
    sent_messages_count: usize,
    received_messages_count: usize,
}

impl Duet {
    fn new(instructions: Vec<Instruction>, sender: Sender<Message>, receiver: Receiver<Message>, program_id: Value) -> Duet {
        let mut registers = Registers::new();
        registers[b'p'] = program_id;

        Duet {
            instructions,
            registers,
            instruction_pointer: 0,
            sender,
            receiver,
            sent_messages_count: 0,
            received_messages_count: 0,
        }
    }

    fn eval(&self, arg: &Argument) -> Value {
        match arg {
            RegisterArgument(reg) => {
                self.registers[*reg]
            }
            ValueArgument(v) => {
                *v
            }
        }
    }

    fn step(&mut self) -> StepResult {
        let instruction = &self.instructions[self.instruction_pointer];
        match instruction {
            Set(reg, arg) => {
                self.registers[*reg] = self.eval(arg);
                self.instruction_pointer += 1;
                Continue
            },
            Add(reg, arg) => {
                self.registers[*reg] += self.eval(arg);
                self.instruction_pointer += 1;
                Continue
            },
            Mul(reg, arg) => {
                self.registers[*reg] *= self.eval(arg);
                self.instruction_pointer += 1;
                Continue
            },
            Mod(reg, arg) => {
                self.registers[*reg] %= self.eval(arg);
                self.instruction_pointer += 1;
                Continue
            },
            Jgz(arg1, arg2) => {
                if self.eval(arg1) > 0 {
                    self.instruction_pointer = (self.instruction_pointer as Value + self.eval(arg2)) as usize;
                } else {
                    self.instruction_pointer += 1;
                }
                Continue
            },
            Snd(arg) => {
                self.sender.send(ValueMessage(self.eval(arg)));
                self.sent_messages_count += 1;
                self.instruction_pointer += 1;
                Continue
            },
            Rcv(reg) => {
                // first, inform the other thread that I will now block on recv
                self.sender.send(Blocked{received_messages_count: self.received_messages_count});

                loop {
                    let message = self.receiver.recv().unwrap();
                    match message {
                        Blocked{received_messages_count} => {
                            if received_messages_count < self.sent_messages_count {
                                // the other thread will not really block, it will still receive messages from me
                                // loop on...
                            } else {
                                // the other thread is blocked after having received my last message, and I am about
                                // to be blocked as well. We have reached the deadlock.
                                return End;
                            }
                        },
                        ValueMessage(value) => {
                            self.registers[*reg] = value;
                            self.received_messages_count += 1;
                            self.instruction_pointer += 1;
                            return Continue;
                        },
                    }
                }
            },

        }
    }

    pub fn run(&mut self, final_sender: Sender<usize>) {
        loop {
            let result = self.step();
            match result {
                Continue => (),
                End => break,
            }
        }
        final_sender.send(self.sent_messages_count);
    }
}

pub fn task2(path: &str) -> usize {
    let text = common::input(path);
    let instructions: Result<Vec<Instruction>, pom::Error> = text
        .lines()
        .into_iter()
        .map(|line| parse_instruction().parse(line.as_bytes()))
        .collect();
    let instructions = instructions.unwrap();

    let (sender0, receiver1) = channel::<Message>();
    let (sender1, receiver0) = channel::<Message>();

    let mut program0 = Duet::new(instructions.clone(), sender0, receiver0, 0);
    let mut program1 = Duet::new(instructions, sender1, receiver1, 1);

    let (final_sender0, final_receiver0) = channel::<usize>();
    let (final_sender1, final_receiver1) = channel::<usize>();

    thread::spawn(move || program0.run(final_sender0));
    thread::spawn(move || program1.run(final_sender1));

    let _ = final_receiver0.recv().unwrap(); // not interested in the result, I just want to wait until the thread finishes
    final_receiver1.recv().unwrap()
}