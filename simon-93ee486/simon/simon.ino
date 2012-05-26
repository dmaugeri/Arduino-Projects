/**
 * simon.ino - a memory game for arduino
 * Daniel and Sam's arduino project
 */

const int SEQ_SIZE = 20;
int mem_seq[SEQ_SIZE];
int seq_index = 0;

const int NONE = 0;
const int RED = 1;
const int GREEN = 2;
const int ORANGE = 3;
const int YELLOW = 4;

const int BUTTON_PIN_1 = 2;
const int BUTTON_PIN_2 = 4;
const int BUTTON_PIN_3 = 6;
const int BUTTON_PIN_4 = 8;

const int LED_PIN_1 = 3;
const int LED_PIN_2 = 5;
const int LED_PIN_3 = 7;
const int LED_PIN_4 = 9;

const int LED_ON_DELAY = 1000;
const int LED_OFF_DELAY = 300;

const int SCORE_MULTIPLIER = 50;
const char *SEPARATOR = "----------------------------------------";

void fill_mem_seq()
{
    for (int i = 0; i < SEQ_SIZE; ++i)
        mem_seq[i] = random(1, 5);
}

void light_led(int led, int led_on_delay, int led_off_delay)
{
    switch(led) {
        case RED:
            digitalWrite(LED_PIN_1, HIGH);
            delay(led_on_delay);
            digitalWrite(LED_PIN_1, LOW);
            break;
        case GREEN:
            digitalWrite(LED_PIN_2, HIGH);
            delay(led_on_delay);
            digitalWrite(LED_PIN_2, LOW);
            break;
        case ORANGE:
            digitalWrite(LED_PIN_3, HIGH);
            delay(led_on_delay);
            digitalWrite(LED_PIN_3, LOW);
            break;
        case YELLOW:
            digitalWrite(LED_PIN_4, HIGH);
            delay(led_on_delay);
            digitalWrite(LED_PIN_4, LOW);
            break;
        default:
            break;
    }

    delay(led_off_delay);
}

int read_switch()
{
    int buttonState1 = digitalRead(BUTTON_PIN_1);
    int buttonState2 = digitalRead(BUTTON_PIN_2);
    int buttonState3 = digitalRead(BUTTON_PIN_3);
    int buttonState4 = digitalRead(BUTTON_PIN_4);

    if (buttonState1 == HIGH) {
        Serial.println("1:HIGH");
        digitalWrite(LED_PIN_1, HIGH);

        while (digitalRead(BUTTON_PIN_1) == HIGH)
            ;

        Serial.println("1:LOW");
        digitalWrite(LED_PIN_1, LOW);

        return RED;
    } else if (buttonState2 == HIGH) {
        Serial.println("2:HIGH");
        digitalWrite(LED_PIN_2, HIGH);

        while (digitalRead(BUTTON_PIN_2) == HIGH)
            ;

        Serial.println("2:LOW");
        digitalWrite(LED_PIN_2, LOW);

        return GREEN;
    } else if (buttonState3 == HIGH) {
        Serial.println("3:HIGH");
        digitalWrite(LED_PIN_3, HIGH);

        while (digitalRead(BUTTON_PIN_3) == HIGH)
            ;

        Serial.println("3:LOW");
        digitalWrite(LED_PIN_3, LOW);

        return ORANGE;
    } else if (buttonState4 == HIGH) {
        Serial.println("4:HIGH");
        digitalWrite(LED_PIN_4, HIGH);

        while (digitalRead(BUTTON_PIN_4) == HIGH)
            ;

        Serial.println("4:LOW");
        digitalWrite(LED_PIN_4, LOW);

        return YELLOW;
    }

    return NONE;
}

void print_seq()
{
    for (int i = 0; i < seq_index; ++i)
        light_led(mem_seq[i], LED_ON_DELAY, LED_OFF_DELAY);
}

int get_score()
{
    return (seq_index - 1) * SCORE_MULTIPLIER;
}

void game_over() {
    Serial.println("You just lost the game...");
    Serial.print("Your score: ");
    Serial.println(get_score());

    int loops = 10;
    for (int i = 0; i < loops; ++i) {
        digitalWrite(LED_PIN_1, HIGH);
        digitalWrite(LED_PIN_2, HIGH);
        digitalWrite(LED_PIN_3, HIGH);
        digitalWrite(LED_PIN_4, HIGH);
        delay(LED_ON_DELAY);
        digitalWrite(LED_PIN_1, LOW);
        digitalWrite(LED_PIN_2, LOW);
        digitalWrite(LED_PIN_3, LOW);
        digitalWrite(LED_PIN_4, LOW);
        delay(LED_ON_DELAY);
    }

    exit(0);
}

void game_completed()
{
    Serial.println("Congratulations, you've completed the game!");
    Serial.print("Your score: ");
    Serial.println(get_score());

    int loops = 10;
    for (int i = 0; i < loops; ++i) {
        light_led(RED, 100, 0);
        light_led(GREEN, 100, 0);
        light_led(ORANGE, 100, 0);
        light_led(YELLOW, 100, 0);
    }

    exit(0);
}

void read_seq()
{
    for (int i = 0; i < seq_index; ++i) {
        int val = read_switch();
        while (val == NONE)
            val = read_switch();

        if (val == mem_seq[i]) {
            Serial.print(i);
            Serial.println(": correct\n");

            if (i == SEQ_SIZE - 1)
                game_completed();
        } else {
            Serial.print(i);
            Serial.println(": incorrect\n");
            game_over();
        }
    }

    ++seq_index;
    Serial.println(SEPARATOR);
}

void setup()
{
    Serial.begin(9600);

    // setup input pins
    pinMode(BUTTON_PIN_1, INPUT);
    pinMode(BUTTON_PIN_2, INPUT);
    pinMode(BUTTON_PIN_3, INPUT);
    pinMode(BUTTON_PIN_4, INPUT);

    // setup output pins
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(LED_PIN_3, OUTPUT);
    pinMode(LED_PIN_4, OUTPUT);

    // use noise from analog pin 0 as seed
    randomSeed(analogRead(0));

    // initialize random sequence of colours
    fill_mem_seq();
}

void loop()
{
    print_seq();
    read_seq();
    delay(LED_ON_DELAY);
}
