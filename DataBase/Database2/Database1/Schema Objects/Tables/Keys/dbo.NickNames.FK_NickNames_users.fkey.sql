﻿ALTER TABLE [dbo].[NickNames] ADD
CONSTRAINT [FK_NickNames_users] FOREIGN KEY ([Login]) REFERENCES [dbo].[users] ([Login]) ON DELETE CASCADE ON UPDATE CASCADE


