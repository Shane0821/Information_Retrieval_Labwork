from bs4 import BeautifulSoup as bs
import requests
import json
import re

url = 'https://so.gushiwen.cn/gushi/tangshi.aspx'
header = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36"
}
fp = requests.get(url=url, headers=header)
soup = bs(fp.text, 'lxml')
res = soup.select('.typecont span')
url_can = []
for each in res:
    each_url = 'https://so.gushiwen.cn'+each.a['href']
    url_can.append(each_url)


def title(soup):
    return soup.select('.main3 .sons h1')[0].text


def author(soup):
    return soup.select('.main3 .sons .source')[0].find_all('a')[0].text


def acient(soup):
    return soup.select('.main3 .sons .source')[0].find_all('a')[1].text


def content(soup):
    return soup.select('.main3 .sons .contson')[0].text


id = 0
for one in url_can:
    id += 1
    try:
        response = requests.get(url=one, headers=header)
    except:
        print('找不到该诗歌链接或网络有问题')
    else:
        one_soup = bs(response.text, 'lxml')
        b = author(one_soup)
        a = title(one_soup)
        c = acient(one_soup)
        d = content(one_soup)
        print('%s 爬取完成!' % a)
        d = d.replace(" ", "")
        d = d.replace("\n", "")
        d = re.sub(u"\\(.*?\\)|\\{.*?}|\\[.*?]", "", d)
        d = re.sub(u"\\（.*?）|\\{.*?}|\\[.*?]|\\【.*?】",
                   "", d.encode('utf-8').decode())
        filepath = './dataset/%s.txt' % id
        with open(filepath, 'w+', encoding='gbk') as f:
            f.write(a + '\n' + b + '\n' + d)
        f.close()
print('over========================================')
