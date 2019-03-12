Feature: An existing customer is able to find products, add them to wishlist, add them to cart. The products in wishlist
  should be visible in next login of customer.

  Scenario: An existing customer logs in
    Given existing customer accoutn is created
    And customer is in login page
    When customer fills email address and password
    Then customer is logged in

  Scenario: An existing customer checks items in cart.
    Given existing customer is logged in
    When customer clicks on Cart
    Then cart page is show

  Scenario: An existing cusotmer checks items in wishlist.
    Given existing customer is logged in
    When customer clicks on wishlist
    Then wishlist is shown

  Scenario: An existing customer searches a product.
    Given existing customer is logged in
    And A customer is in home page
    When customer search product
    Then page with search results is shown

  Scenario Outline: An existing customer open tab with products
    Given existing customer is logged in
    And A customer is in home page
    When customers picks a tab with <group>
    Then page with <group> is shown
    Examples:
      | group       |
      | Desktops    |
      | Tablets     |
      | MP3 players |

  Scenario: An existing customer adds product to cart from home page.
    Given existing customer is logged in
    When existing customer adds product to cart from home page
    Then product is in the cart

  Scenario Outline: An existing customer adds product from category
    Given existing customer is logged in
    And <category> page is shown
    When <product> is added to cart
    Then <product> is in cart
    Examples:
    | category    | product                 |
    | Cameras     | Nikon D300              |

  Scenario: An existing customer adds product to wishlist from home page
    Given existing customer is logged in
    When existing customer adds product to wishlist from home page
    Then product is in wishlist

  Scenario: An existing customer adds product to wishlist form category page
    Given existing customer is logged in
    And category page is shown
    When product is added to wishlist
    Then product is in wishlist

  Scenario: An existing customer deletes product from wishlist
    Given existing customer is logged in
    And product is in wishlist
    And is in wishlist page
    When customer delete product form wishlist
    Then product is deleted from wishlist

  Scenario: An existing customer deletes product from cart in cart page
    Given existing customer is logged in
    And product is in cart
    And is in cart page
    When customer clicks on delete button of product
    Then product is deleted from cart

  Scenario: An existing customer deletes product from cart in home page
    Given existing customer is logged in
    And A customer is in home page
    And product is in cart
    When customer opens cart tab
    And customer clicks on delete button of product in cart tab
    Then product is deleted from cart

  Scenario: An existing customer increases quantity of product in cart.
    Given existing customer is logged in
    And product is in cart
    And is in cart page
    When customer increases quantity of product in cart
    Then the quantity of products in cart is increased

  Scenario: An existing customer decreases quantity of product in cart.
    Given existing customer is logged in
    And product is in cart
    And is in cart page
    When customer decreases quantity of product in cart
    Then the quantity of products in cart is decreased

  Scenario: An existing customer compares products
    Given existing customer is logged in
    When two products are added into comparison list
    Then these two products are shown in product comparison page

  Scenario: An existing customer logs in after log out and checks the wishlist.
    Given existing customer is logged in
    And customer has products in wishlist
    And customer logs out
    When customer logs in
    Then product are shown in wish list