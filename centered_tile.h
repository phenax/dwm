void centered_tile(Monitor *m) {
  int i, count, h, master_w, master_x, master_y, right_y, left_y, stack_w;
  Client *c;
  for (count = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), count++);
  if (count == 0) return;

  /* initialize areas */
  master_w = m->ww;
  master_x = 0;
  master_y = 0;
  stack_w = master_w;

  if (count > m->msplit) {
    /* go mfact box in the center if more than msplit clients */
    master_w = m->msplit ? m->ww * m->mfact : 0;
    stack_w = m->ww - master_w;

    if (count - m->msplit > 1) {
      /* only one client */
      master_x = (m->ww - master_w) / 2;
      stack_w = (m->ww - master_w) / 2;
    }
  }

  right_y = 0;
  left_y = 0;

  int mid_stack = m->msplit + MAX(0, count - m->msplit - 1)/2;
  int left_count = (count - m->msplit)/2;
  int right_count = count - m->msplit - left_count;
  for (i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next), i++) {
    if (i < m->msplit) {
      /* msplit clients */
      h = (m->wh - master_y) / (MIN(count, m->msplit) - i);
      resize(c, m->wx + master_x, m->wy + master_y, master_w - (2*c->bw), h - (2*c->bw), 0);
      master_y += HEIGHT(c);
    } else {
      /* stack clients */
      if (i > mid_stack) {
        h = m->wh / left_count;
        resize(c, m->wx, m->wy + left_y, stack_w - (2*c->bw), h - (2*c->bw), 0);
        left_y += HEIGHT(c);
      } else {
        h = m->wh / right_count;
        resize(c, m->wx + master_x + master_w, m->wy + right_y, stack_w - (2*c->bw), h - (2*c->bw), 0);
        right_y += HEIGHT(c);
      }
    }
  }
}
