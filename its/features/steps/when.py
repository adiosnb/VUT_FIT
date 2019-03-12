from behave import *
from time import sleep

from selenium.webdriver.common.keys import Keys
from selenium.webdriver.support.ui import Select

use_step_matcher("re")


@when(u'customer fills email address and password')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/login")
    context.driver.find_element_by_id("input-email").send_keys("%s@test.dev" % context.username)
    context.driver.find_element_by_id("input-password").send_keys(context.username)
    context.driver.find_element_by_css_selector("input.btn.btn-primary").click()


@when(u'customer clicks on Cart')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-shopping-cart").click()


@when(u'customer clicks on wishlist')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-heart").click()


@when(u'customer search product')
def step_impl(context):
    context.driver.find_element_by_name("search").send_keys("camera")
    context.driver.find_element_by_css_selector("i.fa.fa-search").click()


@when(u'customers picks a tab with Desktops')
def step_impl(context):
    context.driver.find_element_by_xpath("//a[contains(text(),'Desktops')]").click()
    context.driver.find_element_by_css_selector("a.see-all").click()


@when(u'customers picks a tab with Tablets')
def step_impl(context):
    context.driver.find_element_by_xpath("//a[contains(text(),'Tablets')]").click()


@when(u'customers picks a tab with MP3 players')
def step_impl(context):
    context.driver.find_element_by_xpath("//a[contains(text(),'MP3 Players')]").click()
    context.driver.find_element_by_css_selector("li.dropdown.open > div.dropdown-menu > a.see-all").click()


@when(u'existing customer adds product to cart from home page')
def step_impl(context):
    context.driver.get(context.web_url)
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[3]/button/i").click()


@when(u'Nikon D300 is added to cart')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=33")
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[2]/div[2]/button/i").click()
    sleep(1)


@when(u'existing customer adds product to wishlist from home page')
def step_impl(context):
    context.driver.get(context.web_url)
    context.driver.find_element_by_css_selector("button[type=\"button\"] > i.fa.fa-heart").click()


@when(u'product is added to wishlist')
def step_impl(context):
    context.driver.find_element_by_css_selector("button[type=\"button\"] > i.fa.fa-heart").click()


@when(u'customer delete product form wishlist')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/wishlist")
    while True:
        try:
            context.driver.find_element_by_css_selector("a.btn.btn-danger").click()
        except Exception:
            break


@when(u'customer opens cart tab')
def step_impl(context):
    context.driver.get(context.web_url)
    context.driver.find_element_by_id("cart-total").click()


@when(u'customer clicks on delete button of product')
def step_impl(context):
    while True:
        try:
            context.driver.find_element_by_css_selector("i.fa.fa-times-circle").click()
        except Exception:
            break


@when(u'customer clicks on delete button of product from cart tab')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-times").click()


@when(u'customer clicks on delete button of product in cart tab')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-times").click()


@when(u'customer increases quantity of product in cart')
def step_impl(context):
    q_elem = context.driver.find_element_by_xpath("//div[@id='content']/form/div/table/tbody/tr/td[4]/div/input")
    previous_q = int(q_elem.get_attribute("value"))
    context.prev_q = previous_q
    next_q = previous_q + 1
    q_elem.send_keys(Keys.BACK_SPACE)
    q_elem.send_keys(str(next_q))
    context.driver.find_element_by_css_selector("i.fa.fa-refresh").click()


@when(u'customer decreases quantity of product in cart')
def step_impl(context):
    q_elem = context.driver.find_element_by_xpath("//div[@id='content']/form/div/table/tbody/tr/td[4]/div/input")
    previous_q = int(q_elem.get_attribute("value"))
    context.prev_q = previous_q
    next_q = previous_q - 1
    q_elem.send_keys(Keys.BACK_SPACE)
    q_elem.send_keys(str(next_q))
    context.driver.find_element_by_css_selector("i.fa.fa-refresh").click()


@when(u'two products are added into comparison list')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/category&path=24")
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[1]/div/div[2]/div[2]/button[3]/i").click()
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[2]/div[2]/button[3]/i").click()


@when(u'customer logs in')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/login")
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=account/login"
    context.driver.find_element_by_id("input-email").send_keys("%s@test.dev" % context.username)
    context.driver.find_element_by_id("input-password").send_keys(context.username)
    context.driver.find_element_by_css_selector("input.btn.btn-primary").click()


@when(u'customer fills all necessary fiels')
def step_impl(context):
    dr = context.driver
    dr.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/register")
    assert dr.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=account/register"
    dr.find_element_by_id("input-firstname").send_keys(context.username2)
    dr.find_element_by_id("input-lastname").send_keys(context.username2)
    dr.find_element_by_id("input-email").send_keys("%s@test.dev" % context.username2)
    dr.find_element_by_id("input-telephone").send_keys("123456789")
    dr.find_element_by_id("input-address-1").send_keys("street 2345")
    dr.find_element_by_id("input-city").send_keys("New York")
    dr.find_element_by_id("input-postcode").send_keys("12345")
    Select(dr.find_element_by_id("input-country")).select_by_index(2)
    Select(dr.find_element_by_id("input-zone")).select_by_index(1)
    dr.find_element_by_id("input-password").send_keys(context.username2)
    dr.find_element_by_id("input-confirm").send_keys(context.username2)
    dr.find_element_by_name("agree").click()


@when(u'customer clicks to continue button')
def step_impl(context):
    context.driver.find_element_by_css_selector("input.btn.btn-primary").click()


@when(u'customer adds product into chart')
def step_impl(context):
    context.driver.get(context.web_url)
    context.driver.find_element_by_xpath("//div[@id='content']/div[2]/div[2]/div/div[3]/button/i").click()


@when(u'customer click on cart bar')
def step_impl(context):
    context.driver.find_element_by_id("cart-total").click()


@when(u'click on View cart')
def step_impl(context):
    context.driver.find_element_by_xpath("//div[@id='cart']/ul/li[2]/div/p/a/strong").click()


@when(u'customer delete a product from cart')
def step_impl(context):
    raise NotImplementedError(u'STEP: When customer delete a product from cart')


@when(u'clicks on Checkout button')
def step_impl(context):
    context.driver.find_element_by_css_selector("i.fa.fa-share").click()


@when(u'new customer clicks on checkout button')
def step_impl(context):
    context.driver.find_element_by_css_selector("a.btn.btn-primary").click()
