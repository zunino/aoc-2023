use std::fs;

#[derive(Debug)]
struct Card {
    winning_numbers: Vec<u8>,
    own_numbers: Vec<u8>,
    matching_numbers: u8,
    copies: u32,
}

type Cards = Vec<Card>;

fn parse_card(input: &str) -> Card {
    let (_, numbers) = input.split_once(":").unwrap();
    let (winning, own) = numbers.split_once("|").unwrap();
    let mut winning: Vec<u8> = winning
        .split_whitespace()
        .map(|n| n.parse().unwrap())
        .collect();
    let mut own: Vec<u8> = own.split_whitespace().map(|n| n.parse().unwrap()).collect();
    winning.sort();
    own.sort();
    Card {
        winning_numbers: winning,
        own_numbers: own,
        matching_numbers: 0,
        copies: 1,
    }
}

fn read_cards() -> Cards {
    let data = fs::read_to_string("data/d04.txt").expect("Error reading data file");
    data.lines()
        .map(parse_card)
        .collect()
}

fn count_winning_numbers(card: &mut Card) -> u8 {
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
    card.matching_numbers = count;
    count
}

fn part_1_total_points(cards: &mut Cards) -> u32 {
    let mut points = 0u32;
    for card in cards {
        let n_winning = count_winning_numbers(card);
        if n_winning > 0 {
            points += 2u32.pow(n_winning as u32 - 1);
        }
    }
    points
}

// ==[ PART 2]=====

fn part_2_total_scratch_cards(cards: &mut Cards) -> u32 {
    for i in 0..cards.len() {
        for j in i+1..i+1+cards[i].matching_numbers as usize {
            cards[j].copies += cards[i].copies;
        }
    }
    cards.iter().fold(0, |acc, c| acc + c.copies)
}

fn main() {
    let mut cards = read_cards();
    println!("Day 4 - part 1: {}", part_1_total_points(&mut cards));
    println!("Day 4 - part 2: {}", part_2_total_scratch_cards(&mut cards));
}
