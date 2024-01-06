import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    user_id = session["user_id"]

    stocks = db.execute("SELECT symbol, shares_held FROM stocks WHERE user_id = ?", user_id)

    stocks_list = []
    grand_total = 0

    for stock in stocks:
        stock_new = {'symbol': "", 'shares': 0, 'name': "", 'unit_price': 0.00, 'total_price': 0.00}
        stock_new['symbol'] = stock['symbol'].upper()
        stock_new['shares'] = int(stock['shares_held'])
        quote = lookup(stock['symbol'].upper())
        stock_new['name'] = quote['name']
        stock_new['unit_price'] = quote['price']
        stock_new['total_price'] = quote['price'] * int(stock['shares_held'])
        grand_total = grand_total + quote['price'] * int(stock['shares_held'])
        stocks_list.append(stock_new)

    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

    cash_balance = cash[0]['cash']

    grand_total = grand_total + cash_balance

    return render_template("index.html", stocks_list=stocks_list, cash_balance=cash_balance, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = request.form.get("symbol").upper()
        user_id = session["user_id"]

        quote = lookup(symbol)

        if quote is None:
            return apology("symbol does not exist", 400)

        if not request.form.get("shares"):
            return apology("shares must be entered", 400)

        if not request.form.get("shares").isnumeric():
            return apology("shares must be numeric", 400)

        shares = int(request.form.get("shares"))

        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        total_cost = quote["price"] * shares

        cash_balance = cash[0]['cash']

        if cash_balance >= total_cost:
            db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_balance - total_cost, user_id)
            db.execute("INSERT INTO trades (user_id, trade_type, symbol, unit_price, shares, trade_date) VALUES (?, ?, ?, ?, ?, ?)",
                       user_id, "B", symbol, quote["price"], shares, datetime.now())
            trades = db.execute("SELECT * FROM trades WHERE user_id = ?", user_id)
            stocks = db.execute("SELECT shares_held FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
            if len(stocks) == 0:
                db.execute("INSERT INTO stocks VALUES(?, ?, ?)", user_id, symbol, shares)
            else:
                stock_held = stocks[0]['shares_held'] + shares
                db.execute("UPDATE stocks SET shares_held = ? WHERE user_id = ? AND symbol = ?", stock_held, user_id, symbol)
            flash("Buy successful")
            return redirect("/")
        else:
            return apology("Cannot afford number of shares at current price", 400)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    trades = db.execute("SELECT * FROM trades WHERE user_id = ?", session["user_id"])
    return render_template("history.html", trades=trades)


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

    # user requests for quote for a symbol via POST
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)
        else:
            quote = lookup(request.form.get("symbol"))

            if quote is not None:
                return render_template("quoted.html", quote=quote)
            else:
                return apology("symbol does not exist", 400)

    # user visits quote via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    session.clear()

    # When user submits details via Register form
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Ensure password and confirmation are same
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation are not same", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Check if username exists
        if len(rows) == 1:
            return apology("username already exists, try another", 400)

        username = request.form.get("username")
        hash_password = generate_password_hash(request.form.get("password"))

        id = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, hash_password)

        # Remember which user has logged in
        session["user_id"] = id

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol", 400)

        symbol = request.form.get("symbol").upper()

        quote = lookup(symbol)

        if quote is None:
            return apology("symbol does not exist", 400)

        stocks = db.execute("SELECT shares_held FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)

        if len(stocks) == 0:
            return apology("no stocks owned for this symbol", 400)
        else:
            stock_held = stocks[0]['shares_held']

        if not request.form.get("shares"):
            return apology("shares must be entered", 400)

        if not request.form.get("shares").isnumeric():
            return apology("shares must be numeric", 400)

        shares = int(request.form.get("shares"))

        if shares > stock_held:
            return apology("number of stocks sold cannot be more than owned", 400)

        db.execute("INSERT INTO trades (user_id, trade_type, symbol, unit_price, shares, trade_date) VALUES (?, ?, ?, ?, ?, ?)",
                   user_id, "S", symbol, quote["price"], shares, datetime.now())

        if shares == stock_held:
            db.execute("DELETE FROM stocks WHERE user_id = ? AND symbol = ?", user_id, symbol)
        else:
            db.execute("UPDATE stocks SET shares_held = ? WHERE user_id = ? AND symbol = ?", stock_held - shares, user_id, symbol)

        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)

        cash_balance = cash[0]['cash']

        total_gain = quote["price"] * shares

        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash_balance + total_gain, user_id)

        trades = db.execute("SELECT * FROM trades WHERE user_id = ?", user_id)

        flash("Sell successful")
        return redirect("/")
    else:
        symbols = db.execute("SELECT DISTINCT symbol FROM stocks WHERE user_id = ?", user_id)
        return render_template("sell.html", symbols=symbols)


@app.route("/changepswd", methods=["GET", "POST"])
def changepswd():
    """Change user password"""

    # When user submits details via Register form
    if request.method == "POST":

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Ensure password and confirmation are same
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password and confirmation not same", 400)

        id = session["user_id"]

        # Query database for username
        password = db.execute("SELECT hash FROM users WHERE id = ?", id)

        if check_password_hash(password[0]["hash"], request.form.get("password")):
            return apology("new password same as current password", 400)

        new_hash = generate_password_hash(request.form.get("password"))

        db.execute("UPDATE users SET hash = ? WHERE id = ?", new_hash, id)

        # Redirect user to home page
        flash("Password change successful")
        return redirect("/")

    # User reached route via GET
    else:
        return render_template("changepswd.html")

