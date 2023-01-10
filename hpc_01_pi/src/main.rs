#![feature(test)]

extern crate test;

use rand::Rng;
use std::{thread, thread::JoinHandle, vec};

const THREAD_COUNT: usize = 4;
const ARROW_COUNT: usize = 10000000;

fn throw_arrows(capacity: usize) -> usize {
    let mut rng = rand::thread_rng();
    let mut count: usize = 0;

    for _ in 0..capacity {
        let x: f32 = rng.gen();
        let y: f32 = rng.gen();

        if f32::sqrt(x.powf(2.0) + y.powf(2.0)) <= 1.0 {
            count = count + 1;
        }
    }

    count
}

fn get_total(hit: usize, total: usize) -> f32 {
    4.0 * hit as f32 / total as f32
}

pub fn compute_pi_threads(threads: usize, count: usize) -> f32 {
    let mut handles: Vec<JoinHandle<usize>> = vec![];
    let mut result: usize = 0;

    for _ in 0..threads {
        let handler = thread::spawn(move || throw_arrows(count));
        handles.push(handler);
    }

    for handler in handles {
        let res = handler.join().unwrap();
        result = result + res;
    }

    let total = threads * count;
    get_total(result, total)
}

pub fn compute_pi(count: usize) -> f32 {
    let result = throw_arrows(count);
    get_total(result, count)
}

fn main() {
    println!("Pi: {:.6}", compute_pi(THREAD_COUNT * ARROW_COUNT));
    println!("Pi: {:.6}", compute_pi_threads(THREAD_COUNT, ARROW_COUNT));
}

#[cfg(test)]
mod tests {
    use test::Bencher;

    use crate::{compute_pi, compute_pi_threads, THREAD_COUNT};

    #[bench]
    fn bench_compute_pi_100(b: &mut Bencher) {
        b.iter(|| compute_pi(100 * THREAD_COUNT));
    }

    #[bench]
    fn bench_compute_pi_10_000(b: &mut Bencher) {
        b.iter(|| compute_pi(10_000 * THREAD_COUNT));
    }

    #[bench]
    fn bench_compute_pi_100_000(b: &mut Bencher) {
        b.iter(|| compute_pi(100_000 * THREAD_COUNT));
    }

    #[bench]
    fn bench_compute_pi_threads_100(b: &mut Bencher) {
        b.iter(|| compute_pi_threads(THREAD_COUNT, 100));
    }

    #[bench]
    fn bench_compute_pi_threads_10_000(b: &mut Bencher) {
        b.iter(|| compute_pi_threads(THREAD_COUNT, 10_000));
    }

    #[bench]
    fn bench_compute_pi_threads_100_000(b: &mut Bencher) {
        b.iter(|| compute_pi_threads(THREAD_COUNT, 100_000));
    }
}
