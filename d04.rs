use std::fs;

#[derive(Debug)]
struct Card {
    id: u8,
    winning_numbers: Vec<u8>,
    own_numbers: Vec<u8>,
}

fn parse_card(input: &str) -> Card {
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
    return Card {
        id: card_id,
        winning_numbers: winning,
        own_numbers: own,
    };
}

fn read_cards() -> Vec<Card> {
    let data = fs::read_to_string("data/d04.txt").expect("Error reading data file");
    data.lines().map(parse_card).collect()
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

fn part_1_total_points(cards: &Vec<Card>) -> u32 {
    let mut points = 0u32;
    for card in cards {
        let n_winning = count_winning_numbers(card);
        if n_winning > 0 {
            points += 2u32.pow(n_winning as u32 - 1);
        }
    }
    points
}

fn main() {
    let cards = read_cards();
    println!("Day 4 - part 1: {}", part_1_total_points(&cards));
}
