use std::fs;
use std::collections::{HashMap, VecDeque};

#[derive(Debug)]
struct Card {
    winning_numbers: Vec<u8>,
    own_numbers: Vec<u8>,
}

type Cards = HashMap<u8, Card>;

fn parse_card(input: &str) -> (u8, Card) {
    let (card_label, numbers) = input.split_once(":").unwrap();
    let card_id = card_label
        .split_whitespace()
        .nth(1)
        .unwrap()
        .parse()
        .unwrap();
    let (winning, own) = numbers.split_once("|").unwrap();
    let mut winning: Vec<u8> = winning
        .split_whitespace()
        .map(|n| n.parse().unwrap())
        .collect();
    let mut own: Vec<u8> = own.split_whitespace().map(|n| n.parse().unwrap()).collect();
    winning.sort();
    own.sort();
    (card_id, Card {
        winning_numbers: winning,
        own_numbers: own,
    })
}

fn read_cards() -> Cards {
    let data = fs::read_to_string("data/d04.txt").expect("Error reading data file");
    let entries: Vec<(u8, Card)> = data.lines()
        .map(parse_card)
        .collect::<Vec<(u8, Card)>>();
    let mut cards: Cards = HashMap::new();
    for (id, card) in entries.into_iter() {
        cards.insert(id, card);
    }
    cards
}

fn count_winning_numbers(card: &Card) -> u8 {
    let mut count: u8 = 0;
    let mut first_w = 0;
    let mut w_range = &card.winning_numbers[first_w..];
    for n in card.own_numbers.iter() {
        while w_range.len() > 0 && w_range[0] < *n {
            first_w += 1;
            w_range = &card.winning_numbers[first_w..];
        }
        if w_range.len() == 0 {
            break;
        }
        if *n == w_range[0] {
            count += 1;
        }
    }
    count
}

fn part_1_total_points(cards: &Cards) -> u32 {
    let mut points = 0u32;
    for (_, card) in cards {
        let n_winning = count_winning_numbers(card);
        if n_winning > 0 {
            points += 2u32.pow(n_winning as u32 - 1);
        }
    }
    points
}

// ==[ PART 2]=====

fn part_2_total_scratch_cards(cards: &Cards) -> u32 {
    let mut total_cards = cards.len() as u32;
    let mut card_ids: VecDeque<u8> = cards.keys().cloned().collect();
    while card_ids.len() > 0 {
        let card_id = card_ids.pop_front().unwrap();
        let card = cards.get(&card_id).unwrap();
        let n_winning = count_winning_numbers(card);
        if n_winning > 0 {
            for i in 1..=n_winning {
                card_ids.push_back(card_id + i);
            }
            total_cards += n_winning as u32;
        }
    }
    total_cards
}

fn main() {
    let cards = read_cards();
    println!("Day 4 - part 1: {}", part_1_total_points(&cards));
    println!("Day 4 - part 2: {}", part_2_total_scratch_cards(&cards));
}
