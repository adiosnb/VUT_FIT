Feature: A new customer is able to  find some products and add them to cart

  Scenario: A new customer registers new account
    Given A new customer is in register page
    When customer fills all necessary fiels
    And customer clicks to continue button
    Then customer is logged in

  Scenario: A new customer adds an item to chart from home
    Given A new customer is in home page
    When customer adds product into chart
    Then product is in the cart

  Scenario: A new customer searches a product.
    Given A customer is in home page
    When customer search product
    Then page with search results is shown

  Scenario Outline: A new customer open tab with products
    Given A customer is in home page
    When customers picks a tab with <group>
    Then page with <group> is shown
    Examples:
    | group       |
    | Desktops    |
    | Tablets     |
    | MP3 players |

  Scenario: A new customer open cart
    Given A new customer is in page
    When customer clicks on Cart
    Then cart page is show

  Scenario: A new customer delete product from cart
    Given A customer is in cart page
    And  product is in cart
    And is in cart page
    When customer clicks on delete button of product
    Then product is deleted from cart

  Scenario: A new customer deletes product from cart in home page
    Given A customer is in home page
    And product is in cart
    When customer opens cart tab
    And customer clicks on delete button of product from cart tab
    Then product is deleted from cart

  Scenario: A new customer increases quantity of product in cart.
    Given A customer is in cart page
    And product is in cart
    When customer increases quantity of product in cart
    Then the quantity of products in cart is increased

  Scenario: A new customer decreases quantity of product in cart.
    Given A customer is in cart page
    And product is in cart
    And product is in cart
    When customer decreases quantity of product in cart
    Then the quantity of products in cart is decreased

  Scenario: A new customer wants to checkout anywhere from eshop
    Given A new customer is in the eshop
    When clicks on Checkout button
    Then checkout page is shown

  Scenario: A new customer wants to checkout order from cart page
    Given A new customer is in cart page
    And product is in cart
    When new customer clicks on checkout button
    Then checkout page is shown

  Scenario: A new customer compares products
    Given customer is in the eshop
    When two products are added into comparison list
    Then these two products are shown in product comparison page
