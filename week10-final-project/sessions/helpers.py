from flask import redirect, render_template, request, session
from functools import wraps


def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def login_required(f):
    """
    Decorate routes to require login.

    https://flask.palletsprojects.com/en/1.1.x/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function


def get_duration_string(s):
    s_in_m = 60
    m_in_h = 60
    s_in_h = s_in_m * m_in_h
    h = 0
    m = 0
    rem_s = round(s)
    if rem_s >= s_in_h:
        h = int(rem_s / s_in_h)
        rem_s = rem_s - h * s_in_h
    if rem_s >= 60:
        m = int(rem_s / s_in_m)
        rem_s = rem_s - m * s_in_m
    h_str = str(h)
    m_str = str(m)
    if h > 0:
        return h_str + "h " + m_str + "m"
    return str(m) + "m"


def format_number(number):
    """Format value having 2 decimals."""
    if number < 10:
        return "0" + str(number)
    return str(number)
