﻿ALTER TABLE [dbo].[GUserAuthValue] ADD
CONSTRAINT [FK_GUserAuthValue_Users] FOREIGN KEY ([LoginUID]) REFERENCES [dbo].[users] ([LoginUID]) ON DELETE CASCADE ON UPDATE CASCADE

