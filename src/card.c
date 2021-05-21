#include "card.h"

bool isRed(struct Card card) {
    return card.m_color == cc_hearts || card.m_color == cc_diamonds;
}

bool oneLess(struct Card card1, struct Card card2) {
    return card1.m_type+1 == card2.m_type;
}

bool oppositeColor(struct Card card1, struct Card card2) {
    return !isRed(card1) != !isRed(card2);
}

void swapCard(struct Card *pCard1, struct Card *pCard2) {
    if (pCard1 && pCard2) {
        struct Card card = *pCard1;
        *pCard1 = *pCard2;
        *pCard2 = card;
    }
}

const char * typeMark(struct Card card) {
    switch (card.m_type) {
        case ct_ace:    return "A"; break;
        case ct_two:    return "2"; break;
        case ct_three:  return "3"; break;
        case ct_four:   return "4"; break;
        case ct_five:   return "5"; break;
        case ct_six:    return "6"; break;
        case ct_seven:  return "7"; break;
        case ct_eight:  return "8"; break;
        case ct_nine:   return "9"; break;
        case ct_ten:    return "10"; break;
        case ct_jack:   return "J"; break;
        case ct_queen:  return "Q"; break;
        case ct_king:   return "K"; break;
        default:        return "0"; break;
    }

    return "?!";
}

const char * colorMark(struct Card card) {
    switch (card.m_color) {
        case cc_spades:     return "♠"; break;
        case cc_hearts:     return "♥"; break;
        case cc_clubs:      return "♣"; break;
        case cc_diamonds:   return "♦"; break;
        default:            return "?"; break;
    }

    return "!";
}


