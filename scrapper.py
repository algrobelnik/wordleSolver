from selenium import webdriver
from bs4 import BeautifulSoup
import time
import json

def scrape_div_with_selenium(url):
    driver = webdriver.Chrome()

    driver.get(url)

    time.sleep(5)

    page_source = driver.page_source

    driver.quit()

    soup = BeautifulSoup(page_source, 'html.parser')

    outer_div = soup.find('div', class_='_main_kv0wd_1 _game_kv0wd_7')

    if not outer_div:
        print("No outer div found with the specified class name.")
        return

    inner_div = outer_div.find('div', class_='_boards_oakqo_1')

    if not inner_div:
        print("No inner div found with the specified class name.")
        return

    game = {
      "words": [],
      "grades": [],
    }

    for div in inner_div:
        word_array = []
        grade_array = []
        for i, game_div in enumerate(div):
            grade = ""
            if '_sticky_oakqo_124' in game_div.get('class', []):
                break
            if '_scrollIntoView_oakqo_92' in game_div.get('class', []):
                continue
            if '_black_oakqo_109' in game_div.get('class', []):
                grade += "K"
            elif '_yellow_oakqo_112' in game_div.get('class', []):
                grade += "Y"
            elif '_green_oakqo_116' in game_div.get('class', []):
                grade += "G"
            else:
                print(game_div.get('class', [])) 
                grade += "?"; 
            text = game_div.get_text(strip=True)
            position = (int)((i - 1) / 5)
            if position == len(word_array):
                word_array.append(text)
                grade_array.append(grade)
            else:
                word_array[position] += text
                grade_array[position] += grade
        game["words"].append(word_array)
        game["grades"].append(grade_array)

    return game

# Example usage
url = 'https://duotrigordle.com'
game = scrape_div_with_selenium(url)
with open("input.json", "w") as write_file:
    json.dump(game, write_file)
