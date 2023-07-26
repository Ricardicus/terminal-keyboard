#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <chrono>
#include <cmath>
#include <iostream>
#include <map>

// Initialize the map with note frequencies at octave 4
std::map<std::string, double> frequencies = {
    {"C1", 32.70},    {"C#1", 34.65},   {"D1", 36.71},    {"D#1", 38.89},
    {"E1", 41.20},    {"F1", 43.65},    {"F#1", 46.25},   {"G1", 48.99},
    {"G#1", 51.91},   {"A1", 55.00},    {"A#1", 58.27},   {"B1", 61.74},

    {"C2", 65.41},    {"C#2", 69.30},   {"D2", 73.42},    {"D#2", 77.78},
    {"E2", 82.41},    {"F2", 87.31},    {"F#2", 92.50},   {"G2", 97.99},
    {"G#2", 103.83},  {"A2", 110.00},   {"A#2", 116.54},  {"B2", 123.47},

    {"C3", 130.81},   {"C#3", 138.59},  {"D3", 146.83},   {"D#3", 155.56},
    {"E3", 164.81},   {"F3", 174.61},   {"F#3", 185.00},  {"G3", 196.00},
    {"G#3", 207.65},  {"A3", 220.00},   {"A#3", 233.08},  {"B3", 246.94},

    {"C4", 261.63},   {"C#4", 277.18},  {"D4", 293.66},   {"D#4", 311.13},
    {"E4", 329.63},   {"F4", 349.23},   {"F#4", 369.99},  {"G4", 392.00},
    {"G#4", 415.30},  {"A4", 440.00},   {"A#4", 466.16},  {"B4", 493.88},

    {"C5", 523.25},   {"C#5", 554.37},  {"D5", 587.33},   {"D#5", 622.25},
    {"E5", 659.26},   {"F5", 698.46},   {"F#5", 739.99},  {"G5", 783.99},
    {"G#5", 830.61},  {"A5", 880.00},   {"A#5", 932.33},  {"B5", 987.77},

    {"C6", 1046.50},  {"C#6", 1108.73}, {"D6", 1174.66},  {"D#6", 1244.51},
    {"E6", 1318.51},  {"F6", 1396.91},  {"F#6", 1479.98}, {"G6", 1567.98},
    {"G#6", 1661.22}, {"A6", 1760.00},  {"A#6", 1864.66}, {"B6", 1975.53},

    {"C7", 2093.00},  {"C#7", 2217.46}, {"D7", 2349.32},  {"D#7", 2489.02},
    {"E7", 2637.02},  {"F7", 2793.83},  {"F#7", 2959.96}, {"G7", 3135.96},
    {"G#7", 3322.44}, {"A7", 3520.00},  {"A#7", 3729.31}, {"B7", 3951.07},

    {"C8", 4186.01},
};

int currentOctave = 4; // global variable for the current octave

void changeOctave(int change) {
  // Check if 100 ms has passed since the last call
  static std::chrono::steady_clock::time_point lastCallTime;
  std::chrono::steady_clock::time_point currentTime =
      std::chrono::steady_clock::now();
  std::chrono::milliseconds timeSinceLastCall =
      std::chrono::duration_cast<std::chrono::milliseconds>(currentTime -
                                                            lastCallTime);
  if (timeSinceLastCall.count() < 100)
    return;

  currentOctave += change;
  if (currentOctave == 0) {
    currentOctave = 1;
  }

  printf("Octave: %d\n", currentOctave);

  // Update the last call time
  lastCallTime = currentTime;
}

double generateTone(const std::string &note) {
  if (note == "oct+") {
    changeOctave(1);
    return 0.0;
  } else if (note == "oct-") {
    changeOctave(-1);
    return 0.0;
  }

  auto search = note + std::to_string(currentOctave);
  if (note[1] == '+') {
    search = note[0] + std::to_string(currentOctave + 1);
  }
  if (note[1] == '-') {
    search = note[0] + std::to_string(currentOctave - 1);
  }
  auto it = frequencies.find(search);
  if (it != frequencies.end()) {
    return it->second;
  }
  return 0.0;
}

int get_amplitude(int a, int x, int samples) {
  int quanta = samples / 8;
  int attack_end = quanta * 1;
  int decay_end = quanta * 2;
  int sustain_end = quanta * 4;
  int release_end = quanta * 8;

  if (x < attack_end) {
    float y = ((float)x) / quanta * a;
    return (int)y;
  }
  if (x < decay_end) {
    float y = a - (((float)x - attack_end) / quanta) * (1.0 / 3.0 * a);
    return (int)y;
  }
  if (x < sustain_end) {
    return (int)(((float)2.0 / 3.0) * a);
  }
  int p = (int)((float)((float)2.0 / 3.0 * a) -
                ((x - sustain_end) / ((float)(4 * quanta) + 2)) *
                    ((2.0 / 3.0) * a));
  return p;
}

int main() {
  const unsigned SAMPLE_RATE = 44100;
  const unsigned AMPLITUDE = 30000;
  const double TWO_PI = 6.28318;

  sf::Window window(sf::VideoMode(800, 600), "SFML Keyboard");

  std::map<sf::Keyboard::Key, std::string> keys = {
      {sf::Keyboard::A, "C"},    {sf::Keyboard::W, "C#"},
      {sf::Keyboard::S, "D"},    {sf::Keyboard::E, "D#"},
      {sf::Keyboard::D, "E"},    {sf::Keyboard::F, "F"},
      {sf::Keyboard::T, "F#"},   {sf::Keyboard::G, "G"},
      {sf::Keyboard::Y, "G#"},   {sf::Keyboard::H, "A"},
      {sf::Keyboard::U, "A#"},   {sf::Keyboard::J, "B"},
      {sf::Keyboard::K, "C+"},   {sf::Keyboard::Z, "C-"},
      {sf::Keyboard::X, "D-"},   {sf::Keyboard::C, "E-"},
      {sf::Keyboard::V, "F-"},   {sf::Keyboard::B, "G-"},
      {sf::Keyboard::N, "A-"},   {sf::Keyboard::M, "B-"},
      {sf::Keyboard::J, "C"},

      {sf::Keyboard::O, "oct-"}, {sf::Keyboard::P, "oct+"},

  };

  std::map<sf::Keyboard::Key, sf::Sound> sounds;
  std::map<sf::Keyboard::Key, sf::SoundBuffer> buffers;

  for (const auto &key : keys) {
    sounds[key.first] = sf::Sound();
    buffers[key.first] = sf::SoundBuffer();
  }

  sf::Int16 raw[SAMPLE_RATE];
  sf::SoundBuffer buffer;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    for (const auto &key : keys) {
      if (sf::Keyboard::isKeyPressed(key.first)) {
        double increment = generateTone(key.second) / SAMPLE_RATE;
        if (increment == 0)
          continue;
        double x = 0;

        for (int i = 0; i < SAMPLE_RATE; i++) {
          raw[i] = get_amplitude(AMPLITUDE, i, SAMPLE_RATE) * sin(x * TWO_PI);
          x += increment;
        }

        if (!buffers[key.first].loadFromSamples(raw, SAMPLE_RATE, 1,
                                                SAMPLE_RATE)) {
          std::cerr << "Loading failed!" << std::endl;
          return 1;
        }

        // sounds[key.first].stop();
        sounds[key.first].setBuffer(buffers[key.first]);
        sounds[key.first].play();
      }
    }
  }

  return 0;
}
