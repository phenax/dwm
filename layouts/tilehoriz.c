
void
tilehoriz(Monitor *m)
{
	unsigned int i, n, w, mh, mx, tx, ns;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);

	if (n == 0)
		return;

	if(n == 1) {
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	if (n > m->nmaster) {
		mh = m->nmaster ? m->wh * m->mfact : 0;
		ns = m->nmaster > 0 ? 2 : 1;
  } else {
		mh = m->wh - m->gappx;
		ns = 1;
  }

	for (i = 0, mx = tx = m->gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			w = (m->ww - mx) / (MIN(n, m->nmaster) - i) - m->gappx;

			resize(c, m->wx + mx, m->wy + m->gappx, w - 2*c->bw - m->gappx*(5-ns)/2, mh - 2*c->bw, 0);
			if(mx + WIDTH(c) + m->gappx < m->ww)
				mx += WIDTH(c) + m->gappx;
		} else {
			w = (m->ww - tx) / (n - i) - m->gappx;
			resize(c, m->wx + tx, m->wy + mh + m->gappx/ns, w - 2*c->bw - m->gappx*(5-ns)/2, m->wh - mh - (2*c->bw), 0);
			if(tx + WIDTH(c) + m->gappx < m->ww)
				tx += WIDTH(c) + m->gappx;
		}
}

