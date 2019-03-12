### ITS - Projekt 1 
### Author: Adrián Tomašov [xtomas32]
# Testing of customer manipulation of products in the eshop

### Tested features
The BDD tests are focused on product manipulation in eshop. Customer should
be able to register a new account, find product which he would like to buy,
compare this product with similar products, add it to wishlist or cart, He 
is alse able to maintain products in wishlist and products in cart.

All testing scenarios are created twice, for new and existing customer to 
find any difference behavior in UI for logged in non-logged customer. Every
feature is tested from different location in page. For tests with existing 
customer, user is registered at first and then test is run.

  * register a new user
  * logging in an existing user
  * add products to cart
  * add product to wishlist
  * remove product from cart
  * remove product from wishlist
  * search product
  * opening category tab 
  * changing quantity of products in cart
  * wishlist content after logout 
   

### Not tested features - might be added in future
  * whole checkout order process
    * apply gift card
    * apply discount
  * checks the status of order
  
### Appendixes
  * [Appendix 1 - BDD testcaes for new customer](new_cust.feature)
  * [Appendix 2 - BDD testcaes for existing customer](existing_cust.feature)
