#!/bin/python3
# Remote Control
import selenium
import unittest, time, re

# WebDriver
from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities
from selenium.webdriver.common.by import By
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from selenium.common.exceptions import NoAlertPresentException


def before_all(context):
    dp = {'browserName': 'firefox', 'marionette': 'true',
          'javascriptEnabled': 'true'}
    context.driver = webdriver.Remote(
        command_executor='http://mys01.fit.vutbr.cz:4444/wd/hub',
        desired_capabilities=dp)
    context.driver.implicitly_wait(10)
    context.web_url = "http://mys01.fit.vutbr.cz:8051/"
    context.username = "testuser345"
    context.username2 = "newtrolocust102938"


def after_all(context):
    context.driver.get("http://mys01.fit.vutbr.cz:5000/reset/51")
    context.driver.quit()


def before_feature(context, feature):
    context.driver.get(context.web_url)
    try:
        context.driver.find_element_by_css_selector("i.fa.fa-user").click()
        context.driver.find_element_by_xpath("//a[contains(text(),'Logout')]").click()
    except:
        pass


def before_scenario(context, scenario):
    context.driver.get(context.web_url)
