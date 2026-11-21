# Working with GitHub

The whole class works in one repository. Each team works in its own fork and
sends its folder to the course repository with a pull request. The staff
merge it, and your plot appears in everyone's city.

```
 course repository (MajidNami/Computer-Graphics-Fall-2026)
        |  fork once                       ^  pull request per phase
        v                                  |
 your fork on GitHub  --- git push --->  your fork's branch
        |  git clone                       ^
        v                                  |
 your computer  ----------- git commit ----+
```

Run the commands on this page in the project folder (in Visual Studio:
**View → Terminal**). They are written for Linux and macOS; on Windows write
`py tools\citylab.py` instead of `python3 tools/citylab.py`. The `git`
commands are the same everywhere. Setting up Git and cloning are explained
step by step in the [README](../README.md#requirements).

## Once per team

1. **Make sure your team is in `city/registry.json`.** The staff put it there
   with your topic. You do not send them your GitHub logins: you write them
   in your own `team.json` (step 4).
2. **Fork.** One member presses **Fork** on the course repository. Add the
   other member as a collaborator of the fork (*Settings → Collaborators*),
   so you both push to the same fork.
3. **Clone and connect the course repository** as `upstream`:

   ```sh
   git clone https://github.com/<your-login>/Computer-Graphics-Fall-2026.git
   cd Computer-Graphics-Fall-2026
   git remote add upstream https://github.com/MajidNami/Computer-Graphics-Fall-2026.git
   ```

4. **Create your folder** and push it:

   ```sh
   python3 tools/citylab.py new team07
   git checkout -b team07
   git add teams/team07
   git commit -m "team07: start from the template"
   git push -u origin team07
   ```

   Before you push, open `teams/team07/team.json` and write each member's
   GitHub login in `"github"`. CI uses these logins to know which folder is
   yours. The staff check that they are yours when they review your first
   pull request; from then on only the logins already merged count, so
   nobody else can change your folder.

Work on a branch named after your team, not on `main`. Your fork's `main`
then stays a clean copy of the course repository, which makes updating easy.

## Every day

```sh
python3 tools/citylab.py run team07      # try it
python3 tools/citylab.py check team07    # what the graders see
git add teams/team07
git commit -m "team07: the hangar roof"
git push
```

- Commit small steps with clear messages. Both members commit their own
  work.
- Add only `teams/team07/`. If `git status` shows other changed files,
  something on your computer changed them (an editor, a build); leave them
  out of your commit.
- Never commit `build/`, `out/`, `dist/` or `settings.json`. They are already in
  `.gitignore`.

## Staying up to date

The staff fix bugs and add features during the term. Bring them into your
branch:

```sh
git fetch upstream
git merge upstream/main
```

Because you only change your own folder, this merge never has conflicts.
If it does, someone changed a file outside `teams/team07/`. Ask a TA before
you resolve it.

## Pull requests

When a phase is ready, or whenever you want your plot in the shared city:

1. Push your branch, then open a pull request on GitHub from
   `<your-login>:team07` into `MajidNami:main`.
2. Fill in the template: the team, the phase, what is new, one or two
   screenshots from `build/check/team07/`.
3. CI runs three jobs on it:

   | Job | What it checks | On your computer |
   |---|---|---|
   | `boundary` | the pull request changes only your own team folder | `git diff --name-only upstream/main...HEAD` |
   | `lint` | the [project rules](rules.md) in your code | `python3 tools/citylab.py lint team07` |
   | `check` | builds the city on Linux and runs the check for your team; the report and screenshots are attached to the run | `python3 tools/citylab.py check team07` |

4. A TA reviews it. Push more commits to the same branch to answer review
   comments; the pull request updates by itself.
5. When CI is green and the review is done, the staff merge it.

A red job has a link to its log. Open it and read the **first** error. The
same commands run on your computer, so you can fix and test before you push
again.

## Submitting a phase

A phase is submitted in two places:

1. **GitHub:** the pull request.
2. **Courses:** the zip made by

   ```sh
   python3 tools/citylab.py pack team07 --phase 1
   ```

   It runs the check, then writes `dist/CG_PROJ_P1_team07.zip` with your
   folder, the check report, the screenshots and `COMMIT.txt` (the commit
   the zip was made from). Commit and push first: if your folder has
   uncommitted changes, `pack` warns you, because the zip and GitHub would
   not agree.

The graders look at the commit named in `COMMIT.txt`.

## When something goes wrong

| Problem | Fix |
|---|---|
| `boundary` says your login is not listed for any team | Write your GitHub login in `"members"` → `"github"` of `teams/team07/team.json` and push. This works for the first pull request of a team that is in `city/registry.json`. After that, only a teammate already in the merged `team.json` can add a login; otherwise ask a TA. |
| `boundary` says the pull request changes other files | `git diff --name-only upstream/main...HEAD` lists them. Restore each one with `git checkout upstream/main -- <file>` and commit. |
| `check` is red on GitHub but green on your computer | Read the log: CI builds on Linux, so look for an upper/lower-case file name (`Brick.png` vs `brick.png`) or a Windows-only header. |
| You committed a large file by mistake | Delete it and commit. It stays in the history, so tell a TA before you open the pull request. |
| Two members edited the same file | `git pull` merges; Git marks the conflicting lines with `<<<<<<<`. Keep the right version, delete the markers, commit. |
