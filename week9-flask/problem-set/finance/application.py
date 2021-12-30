import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True


# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]
    cash = db.execute("SELECT cash FROM users WHERE id = (?)", user_id)[0]["cash"]
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = (?) GROUP BY symbol HAVING SUM(shares) > 0;", user_id)
    total = cash
    for stock in portfolio:
        stock_info = lookup(stock["symbol"])
        stock["name"] = stock_info["name"]
        stock["price"] = float(stock_info["price"])
        stock_total = stock["shares"] * float(stock_info["price"])
        stock["total"] = stock_total
        total += stock_total

    return render_template("index.html", portfolio=portfolio, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        if not symbol:
            return apology("Missing symbol")

        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol")
        share_price = quote["price"]

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing shares")

        try:
            shares = float(shares)
        except:
            return apology("Value must be an integer")

        if shares < 1 or not (shares).is_integer():
            return apology("Value must be an integer of 1 or greater")
        shares = int(shares)

        user_id = session["user_id"]
        cost = share_price * shares
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", user_id)[0]["cash"]
        if cost > cash:
            return apology("Can't afford")

        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES ((?), (?), (?), (?))",
                   user_id, symbol, shares, share_price)
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)", cash - cost, user_id)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute("SELECT * FROM transactions")
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        quote = lookup(symbol)
        if not quote:
            return apology("Invalid symbol")

        return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username").lower()
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        users = db.execute("SELECT * FROM users WHERE username = (?)", username)
        if not username:
            return apology("Missing username")

        if len(users) == 1:
            return apology("Username is already taken")

        if not password:
            return apology("Missing password")

        if not confirmation:
            return apology("Missing password confirmation")

        if not password == confirmation:
            return apology("Password and confirmation not matching")

        db.execute("INSERT INTO users (username, hash) VALUES ((?), (?))", username, generate_password_hash(password))
        return redirect("/login")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Missing symbol")

        shares = request.form.get("shares")
        if not shares:
            return apology("Missing shares")

        try:
            shares = float(shares)
        except:
            return apology("Value must be an integer")

        if shares < 1 or not (shares).is_integer():
            return apology("Value must be an integer of 1 or greater")
        shares = int(shares)

        stock_in_portfolio = False
        portfolio = db.execute(
            "SELECT symbol, SUM(shares) shares FROM transactions WHERE user_id = (?) GROUP BY symbol HAVING SUM(shares) > 0;", user_id)
        for stock in portfolio:
            if stock["symbol"] == symbol and shares > stock["shares"]:
                return apology("Too many shares")

            if stock["symbol"] == symbol:
                stock_in_portfolio = True

        if not stock_in_portfolio:
            return apology("You don't own any (" + symbol + ") stocks.")

        quote = lookup(symbol)
        if not quote:
            return apology("Failed to fetch stock info")
        share_price = quote["price"]
        sell_total = share_price * shares
        db.execute(
            "INSERT INTO transactions (user_id, symbol, shares, price) VALUES ((?), (?), (?), (?))", user_id, symbol, -shares, share_price)
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", user_id)[0]["cash"]
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)", cash + sell_total, user_id)
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        portfolio = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = (?) GROUP BY symbol HAVING SUM(shares) > 0;", user_id)
        return render_template("sell.html", portfolio=portfolio)


@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        amount = float(request.form.get("amount"))

        if not amount:
            return apology("Missing amount")

        if not amount >= 0:
            return apology("Number must be positive")

        user_id = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = (?)", user_id)[0]["cash"]
        db.execute("UPDATE users SET cash = (?) WHERE id = (?)", cash + amount, user_id)

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("deposit.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
