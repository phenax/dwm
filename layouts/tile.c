
void
tile(Monitor *m)
{
	unsigned int i, n, h, mw, my, ty, ns;
	Client *c;

	for (n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), n++);
	if (n == 0)
		return;
	if (n == 1){
		c = nexttiled(m->clients);
		resize(c, m->wx, m->wy, m->ww - 2 * c->bw, m->wh - 2 * c->bw, 0);
		return;
	}

	if (n > m->nmaster) {
		mw = m->nmaster ? m->ww * m->mfact : 0;
		ns = m->nmaster > 0 ? 2 : 1;
  } else {
		mw = m->ww - m->gappx;
		ns = 1;
  }
	for (i = 0, my = ty = m->gappx, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++)
		if (i < m->nmaster) {
			h = (m->wh - my) / (MIN(n, m->nmaster) - i) - m->gappx;
			resize(c, m->wx + m->gappx, m->wy + my, mw - 2*c->bw - m->gappx*(5-ns)/2, h - 2*c->bw, 0);
			if(my + HEIGHT(c) + m->gappx < m->wh)
				my += HEIGHT(c) + m->gappx;
		} else {
			h = (m->wh - ty) / (n - i) - m->gappx;
			resize(c, m->wx + mw + m->gappx/ns, m->wy + ty, m->ww - mw - (2*c->bw) - m->gappx*(5-ns)/2, h - 2*c->bw, 0);
			if(ty + HEIGHT(c) + m->gappx < m->wh)
				ty += HEIGHT(c) + m->gappx;
		}
}

