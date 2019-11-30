import os
import json
import urllib.request
from selenium import webdriver

class Crawler:

        def __init__(self):
            self.keyword = str(input('keyword... : '))
            self.dirPath = ""
        
        def createNewDirectory(self):
            cwd = os.getcwd()
            self.dirPath = os.path.join(cwd, self.keyword)

            if not os.path.exists(self.dirPath):
                os.mkdir(self.dirPath)

        def downloadImage(self,browser):
            elements = browser.find_elements_by_xpath('//div[contains(@class,"rg_meta")]')
            element_size = len(elements)

            for idx, element in enumerate(elements):

                img = json.loads(element.get_attribute('innerHTML'))["ou"]
                img_type = json.loads(element.get_attribute('innerHTML'))["ity"]

                try:
                    raw_img = urllib.request.urlopen(img).read()
                    filePath = self.dirPath + '/' + str(idx) + '.' + img_type
                    with open(filePath, mode="wb") as file:
                        file.write(raw_img)
                except:
                    pass
    
                if idx % 10 == 0:
                    print('{}% completed ...'.format(idx * 100 / element_size))
            browser.close()


        def createURL(self):
            url = 'http://www.google.com/search?q=' + self.keyword + '&source = lnms&tbm=isch'
            return url

        def searchURL(self, url):
            browser = webdriver.Chrome("C:\chromedriver")
            browser.get(url)

            for _ in range(1):
                browser.execute_script('window.scrollBy(0,10000)')

            return browser
        


        def run(self):
            self.createNewDirectory()
            url = self.createURL()
            browser = self.searchURL(url)
            self.downloadImage(browser)

newCrawler = Crawler()
newCrawler.run()
