﻿ALTER TABLE [dbo].[GPointBagRecommend] ADD
CONSTRAINT [FK_GPointBagRecommend_GPointBag] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[GPointBag] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE


