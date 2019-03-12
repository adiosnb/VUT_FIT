from behave import *
from selenium.webdriver.support.ui import Select
from selenium.common.exceptions import NoSuchElementException
from time import sleep

use_step_matcher('re')


@given(u'existing customer accoutn is created')
def step_impl(context):
    dr = context.driver
    dr.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/register")
    dr.find_element_by_id("input-firstname").send_keys(context.username)
    dr.find_element_by_id("input-lastname").send_keys(context.username)
    dr.find_element_by_id("input-email").send_keys("%s@test.dev" % context.username)
    dr.find_element_by_id("input-telephone").send_keys("123456789")
    dr.find_element_by_id("input-address-1").send_keys("street 2345")
    dr.find_element_by_id("input-city").send_keys("New York")
    dr.find_element_by_id("input-postcode").send_keys("12345")
    Select(dr.find_element_by_id("input-country")).select_by_index(2)
    Select(dr.find_element_by_id("input-zone")).select_by_index(1)
    dr.find_element_by_id("input-password").send_keys(context.username)
    dr.find_element_by_id("input-confirm").send_keys(context.username)
    dr.find_element_by_name("agree").click()
    dr.find_element_by_css_selector("input.btn.btn-primary").click()
    sleep(2)
    try:
        dr.find_element_by_css_selector("i.fa.fa-user").click()
        dr.find_element_by_xpath("//a[contains(text(),'Logout')]").click()
    except NoSuchElementException:
        pass


@given(u'customer is in login page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/login")


@given(u'existing customer is logged in')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/login")
    if context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=account/login":
        context.driver.find_element_by_id("input-email").send_keys("%s@test.dev" % context.username)
        context.driver.find_element_by_id("input-password").send_keys(context.username)
        context.driver.find_element_by_css_selector("input.btn.btn-primary").click()


@given(u'A customer is in home page')
def step_impl(context):
    context.driver.get(context.web_url)


@given(u'Tablets page is shown')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=57")


@given(u'Cameras page is shown')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=33")


@given(u'category page is shown')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=20")

@given(u'is in wishlist page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/wishlist")


@given(u'product is in wishlist')
def step_impl(context):
    context.driver.get(context.web_url)
    context.driver.find_element_by_css_selector("button[type=\"button\"] > i.fa.fa-heart").click()


@given(u'is in cart page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")


@given(u'product is in cart')
def step_impl(context):
    current_url = context.driver.current_url
    context.driver.get(context.web_url)
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[3]/button/i").click()
    context.driver.get(current_url)

@given(u'customer is in the eshop')
def step_impl(context):
    context.driver.get(context.web_url)


@given(u'customer has products in wishlist')
def step_impl(context):
    current_url = context.driver.current_url
    context.driver.get(context.web_url)
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[3]/button[2]/i").click()
    context.driver.get(current_url)


@given(u'customer logs out')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-user").click()
    context.driver.find_element_by_xpath("//a[contains(text(),'Logout')]").click()

@given(u'A new customer is in register page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/register")

@given(u'A new customer is in home page')
def step_impl(context):
    context.driver.get(context.web_url)

@given(u'A new customer is in page')
def step_impl(context):
    context.driver.get(context.web_url)


@given(u'A customer is in cart page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")


@given(u'A new customer is in the eshop')
def step_impl(context):
    context.driver.get(context.web_url)


@given(u'A new customer is in cart page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
