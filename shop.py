from flask import (
    Blueprint, flash, g, redirect, render_template, request, url_for, session
)
from werkzeug.exceptions import abort

from flaskr.auth import login_required
from flaskr.db import get_db

sh = Blueprint('shop', __name__)

@sh.route('/')
@login_required
def index():
    db = get_db()
    user_id = session.get('user_id')
    role = db.execute(
                'SELECT role_id FROM users WHERE username = ?', (user_id,)
    ).fetchone()
    return render_template('shop/index.html', role=role)

@sh.route('/additem', methods=('GET', 'POST'))
@login_required
def additem():
    if request.method == 'POST':
        item = request.form['item']
        unit = request.form['unit']
        total_qty = request.form['total_qty']
        
        db = get_db()
        error = None

        if not item:
            error = 'Item is required.'
        elif not unit:
            error = 'Unit is required.'
        elif not total_qty:
            error = 'Total Quantity is required.'

        if error is None:
            db.execute(
                "INSERT INTO items (name, unit, total_qty) VALUES (?, ?, ?)",
                (item, unit, total_qty),
            )
            db.commit()
            flash("Item added successfully.")
        else:
            flash(error)

    return render_template('shop/additem.html')


@sh.route('/viewitems')
@login_required
def viewitems():
    db = get_db()
    items = db.execute(
                'SELECT id, name, unit, total_qty FROM items'
    ).fetchall()
    
    return render_template('shop/viewitems.html', items=items )


@sh.route('/viewusers')
@login_required
def viewusers():
    db = get_db()
    users = db.execute(
                'SELECT * FROM users'
    ).fetchall()
    
    return render_template('shop/viewusers.html', users=users)


@sh.route('/asgrole', methods=('GET', 'POST'))
@login_required
def asgrole():
    if request.method == 'POST':
        username = request.form['username']
        role_id = request.form['role_id']

        db = get_db()
        error = None

        if not username:
            error = 'Username must be entered'
        elif not role_id:
            error = 'Role must be selected.'
        
        if error is None:
            db.execute(
                "UPDATE users SET role_id = ? WHERE username = ?",
                (role_id, username),
            )
            db.commit()

            flash("User role change successful") 
        else:
            flash(error)

    return render_template('shop/asgrole.html')


@sh.route('/buyitems')
@login_required
def buyitems():
    db = get_db()
    buys = db.execute(
                'SELECT id, name, unit, total_qty FROM items'
    ).fetchall()
    
    return render_template('shop/buyitems.html', buys=buys)


