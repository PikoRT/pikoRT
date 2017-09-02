.. _contribute:

Contribute to Piko/RT
=====================

We welcome everyone to help to develop Piko/RT.
This will help you to find were to help, and how to submit a pull request
to GitHub.


How to contribute
-----------------

There are a variety of ways to contribute to this project.
Here are the list what you may help:

* Write documentation.
* Report issue when you occure.
* Implement new feature.
* Code review -- this is always helpful.
* Make a bug fixed and send the patches.
* Adding new test.
* Integrate new feature -- benchmark, TCP stack, GNU utilt...etc.
* Port to new paltform.


Create your GitHub account
--------------------------

If you have a GitHub account, please skip this section.

If you don't have, please follow these step:

* Click `Join GitHub <https://github.com/join>`_
* Fill the form
* Confirm your email


Fork Piko/RT repo
-----------------

* Go to `Piko/RT repo <https://github.com/piko-rt/pikoRT>`_
* Click the buttom "Fork" at the right top side.
* Done!


Clone the Piko/RT repo
----------------------

You will need a copy of Piko/RT to work on the code::

    $ git clone https://github.com/<your-username>/pikoRT


Create a local Git branch
-------------------------

If you make some changed to the project, and want to share with everyone,
you will need to upload to GitHub.

First, create a new branch::

    $ git checkout -b <branch-name>

Then commit the changed in this branch::

    $ git add <your-changed>
    $ git commit

Push it to your fork::

    $ git push -u origin <branch-name>


Publish your pull request
-------------------------

You will then need to go to `Piko/RT's repo <https://github.com/piko-rt/pikoRT>`_,
Click ``Pull reuqests``, Click green button ``New pull request``.

Then reference this tutorial from GitHub:
`Creating a pull request from a fork <https://help.github.com/articles/creating-a-pull-request-from-a-fork/>`_


Git remotes
-----------

Your local git repo's origin is your fork. So if you do something like this::

    $ git pull

You won't get the latest commit from Piko/RT.

You will need to add a upstream, fetch from it, then rebase::

    $ git remote add upstream git@github.com:Piko-RT/pikoRT.git
    $ git fetch upstream
    $ git rebase upstream/master

.. note::

    Before you rebase, you will need to stash all your change via ``git stash``.
    After rebase, if you want your stash file back, you can type in
    ``git stash pop`` to get back your stash.
