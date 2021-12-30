import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, get_duration_string
from datetime import date as dateclass

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


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///sessions.db")


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show sessions for a day"""
    start_input = False
    end_input = False
    session_id = False
    total_duration = 0
    session["date"] = str(dateclass.today())

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        session["date"] = request.form.get("date")

    date = session["date"]

    last_session = db.execute("SELECT * FROM sessions WHERE user_id = (?) ORDER BY id DESC LIMIT 1", session["user_id"])
    if last_session and not last_session[0]["end"]:
        start_input = last_session[0]["start"].replace(" ", "T")
        session_id = last_session[0]["id"]

    sessions = db.execute("SELECT id, time(start) start, time(end) end, (julianday(end) - julianday(start)) * 24 * 60 * 60 as duration FROM sessions WHERE user_id = (?) AND start LIKE '%" + date + "%' ORDER BY start", session["user_id"])
    for row in sessions:
        row["start"] = row["start"][:-3]
        if row["end"]:
            row["end"] = row["end"][:-3]
            total_duration += int(round(row["duration"]))
            row["duration"] = get_duration_string(row["duration"])
        else:
            row["end"] = "-"
            row["duration"] = "-"
    total_duration = get_duration_string(total_duration)
    return render_template("index.html", sessions=sessions, total_duration=total_duration, start_input=start_input, session_id=session_id, date=date)


@app.route("/start-session", methods=["POST"])
@login_required
def start_session():
    last_session = db.execute("SELECT * FROM sessions WHERE user_id = (?) ORDER BY id DESC LIMIT 1", session["user_id"])
    if last_session and not last_session[0]["end"]:
        return redirect("/")
    start = request.form.get("start").replace("T", " ")
    db.execute("INSERT INTO sessions (user_id, start) VALUES ((?), (?))", session["user_id"], start)
    return redirect("/")


@app.route("/update-session-start", methods=["POST"])
@login_required
def update_session_start():
    session_id = request.form.get("session_id")
    start = request.form.get("start").replace("T", " ")
    db.execute("UPDATE sessions SET start = (?) WHERE id = (?)", start, session_id)
    return redirect("/")


@app.route("/update-session-end", methods=["POST"])
@login_required
def update_session_end():
    session_id = request.form.get("session_id")
    start = db.execute("SELECT start FROM sessions WHERE id = (?)", session_id)[0]["start"]
    end = request.form.get("end").replace("T", " ")
    if end >= start:
        db.execute("UPDATE sessions SET end = (?) WHERE id = (?)", end, session_id)
    return redirect("/")


@app.route("/delete-session", methods=["POST"])
@login_required
def delete_session():
    session_id = request.form.get("id")
    sessions = db.execute("SELECT * FROM sessions WHERE id = (?)", session_id)
    if len(sessions) > 0:
        db.execute("DELETE FROM sessions WHERE id = (?)", session_id)
        return redirect("/")
    return render_template("index.html", alert_msg="This session was not found in the database.", alert_type="danger")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = (?)", request.form.get("username").lower())

        # Ensure username exists
        if len(rows) != 1:
            return render_template("login.html", alert_msg="Invalid username. Try again.", alert_type="danger")

        # Ensure password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return render_template("login.html", alert_msg="Incorrect password. Try again.", alert_type="danger")

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


@app.route("/signup", methods=["GET", "POST"])
def signup():
    """Register user"""
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username").lower()
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        users = db.execute("SELECT * FROM users WHERE username = (?)", username)

        # Ensure username is available
        if len(users) > 0:
            return render_template("signup.html", alert_msg="Username is already taken", alert_type="danger")

        if not password == confirmation:
            return apology("Password and confirmation not matching")

        db.execute("INSERT INTO users (username, hash) VALUES ((?), (?))", username, generate_password_hash(password))
        return render_template("login.html", alert_msg="Congratulations! Your account has been successfully created. Please log in.", alert_type="success")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("signup.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
