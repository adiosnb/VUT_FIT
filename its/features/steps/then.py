from behave import *
from selenium.common.exceptions import NoSuchElementException
from time import sleep
use_step_matcher("re")


@then(u'customer is logged in')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/login")
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=account/account"

@then(u'cart page is show')
def step_impl(context):
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart"

@then(u'wishlist is shown')
def step_impl(context):
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=account/wishlist"


@then(u'page with search results is shown')
def step_impl(context):
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=product/search&search=camera"


@then(u'page with Desktops is shown')
def step_impl(context):
    assert context.driver.find_element_by_css_selector("h2").text == "Desktops"

@then(u'page with Tablets is shown')
def step_impl(context):
    assert context.driver.find_element_by_css_selector("h2").text == "Tablets"


@then(u'page with MP3 players is shown')
def step_impl(context):
    assert context.driver.find_element_by_css_selector("h2").text == "MP3 Players"


@then(u'product is in the cart')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
    sleep(1)
    elem = context.driver.find_element_by_css_selector("div.table-responsive")
    elem_text = elem.text
    assert elem_text.find("iPhone") > -1


@then(u'Nikon D300 is in cart')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
    sleep(1)
    assert context.driver.find_element_by_css_selector("div.table-responsive").text.find("Nikon") > -1


@then(u'product is in wishlist')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/wishlist")
    assert context.driver.find_element_by_css_selector("div.table-responsive").text.find("MacBook") > -1

@then(u'product is deleted from wishlist')
def step_impl(context):
    try:
        context.driver.find_element_by_css_selector("div.table-responsive")
        raise AssertionError
    except NoSuchElementException:
        pass

@then(u'product is deleted from cart')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/cart")
    try:
        elem = context.driver.find_element_by_css_selector("div.table-responsive")
        raise AssertionError
    except NoSuchElementException:
        pass


@then(u'the quantity of products in cart is increased')
def step_impl(context):
    q_elem = context.driver.find_element_by_xpath("//div[@id='content']/form/div/table/tbody/tr/td[4]/div/input")
    curr_q = int(q_elem.get_attribute("value"))
    assert curr_q > context.prev_q

@then(u'the quantity of products in cart is decreased')
def step_impl(context):
    q_elem = context.driver.find_element_by_xpath("//div[@id='content']/form/div/table/tbody/tr/td[4]/div/input")
    curr_q = int(q_elem.get_attribute("value"))
    assert curr_q < context.prev_q


@then(u'these two products are shown in product comparison page')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=product/compare")
    first_name = context.driver.find_element_by_xpath("//div[@id='content']/table/tbody/tr/td[2]").text
    second_name = context.driver.find_element_by_xpath("//div[@id='content']/table/tbody/tr/td[3]").text
    assert first_name.find("HTC Touch HD") > -1
    assert second_name.find("iPhone") > -1

@then(u'product are shown in wish list')
def step_impl(context):
    context.driver.get("http://mys01.fit.vutbr.cz:8051/index.php?route=account/wishlist")
    try:
        context.driver.find_element_by_css_selector("div.table-responsive")
    except:
        raise AssertionError

@then(u'account is created')
def step_impl(context):
    raise NotImplementedError(u'STEP: Then account is created')


@then(u'product is in a chart')
def step_impl(context):
    raise NotImplementedError(u'STEP: Then product is in a chart')


@then(u'page with cart content is shown')
def step_impl(context):
    raise NotImplementedError(u'STEP: Then page with cart content is shown')


@then(u'checkout page is shown')
def step_impl(context):
    assert context.driver.current_url == "http://mys01.fit.vutbr.cz:8051/index.php?route=checkout/checkout"
