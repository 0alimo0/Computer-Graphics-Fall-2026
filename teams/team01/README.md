# team01 · Observatory & Planetarium

This folder belongs to your team alone. Do not change anything outside
`teams/team01/`; CI rejects a pull request that does.

## Members

| Name | Student ID | GitHub login |
|---|---|---|
| | | |
| | | |

## Place

- Plot: CU-1
- Place from `city/places.json`: Observatory & Planetarium (`observatory`)
- The idea: A paved approach splits into two paths inside a low boundary wall.
  The west branch leads to the observatory, which has a 32 m wide cylindrical
  base and a roughly 28 m telescope projecting through an open slit in its
  dome. The east branch leads to the future planetarium.

## Planetarium site

The four flush corner markers on the east lawn outline a 28 × 32 m footprint.
On CU-1, its plot-local center is `(20, 0, -18)`, with `x` from 6 to 34 m
and `z` from -34 to -2 m. The center and the street-facing edge line up with
the observatory's cylindrical base. Face the entrance toward `+z` so it meets
the paved spur centered on `x = 20` at the front edge. The footprint has no
colliders.

`PlotLayout` in `src/Plot.h` exposes the center and half-extents for placement.
Add the planetarium drawing alongside the observatory placement in `src/MyZone.cpp`.

## Phases

| Phase | What we built | Status |
|---|---|---|
| 1 | Lawn, paved approaches, observatory exterior and telescope, reserved planetarium site | In progress |
| 2 | | |
| 3 | | |

## Keys

| Key | What it does |
|---|---|
| | |

## Sources and licences

List every image or file you did not make yourselves, with its address and licence.

Codex assisted with the site and observatory exterior code.

The observatory exterior uses the [Keck Maunakea Summit gallery](https://keckobservatory.org/maunakea-summit/)
as a visual reference for the pale dome. All scene geometry is drawn in code.
